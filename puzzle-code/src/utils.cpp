#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <zip.h>

#include "./include/utils.h"

namespace fs = std::filesystem;

void utils_generate_file(filepath_t filepath, const std::string &output_body)
{
  if (FLAGS & ANS_ONLY) {
    return;
  }
  std::ofstream out(filepath, std::ios::binary);
  out << output_body;
  out.close();
  if (out.fail()) {
    throw std::runtime_error("could not write " + filepath);
  }
}

void utils_mkdir(filepath_t filepath)
{
  if (FLAGS & ANS_ONLY) {
    return;
  }
  fs::create_directories(filepath);
}

void utils_remove_all(filepath_t filepath)
{
  if (FLAGS & ANS_ONLY) {
    return;
  }
  fs::remove_all(filepath);
}

int utils_rng_roll(int min, int max, seed_t &seed)
{
  if (min > max) {
    std::swap(min, max);
  }

  // splitmix64. The seed moves forward by a fixed odd constant, so different seeds never merge
  // into the same sequence of numbers.
  seed += 0x9E3779B97F4A7C15ull;
  uint64_t z = seed;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  z ^= z >> 31;

  // The range is at most 2^32, so the bias from % is negligible.
  uint64_t range = (uint64_t)((int64_t)max - (int64_t)min) + 1;
  return (int)((int64_t)min + (int64_t)(z % range));
}

bool utils_chance(int percentage, seed_t &seed)
{
  return utils_rng_roll(1, 100, seed) <= percentage;
}

seed_t utils_derive_seed(seed_t seed, const std::string &name)
{
  // Mixes in the name with FNV-1a. utils_rng_roll() hashes the seed, so seeds that differ at all
  // give unrelated numbers.
  for (unsigned char c : name) {
    seed ^= c;
    seed *= 0x100000001B3ull;
  }
  return seed;
}

seed_t utils_roll_seed(void)
{
  std::random_device device;
  return ((seed_t)device() << 32) ^ device() ^ (seed_t)std::time(nullptr);
}

seed_t utils_seed_from_email(const std::string &email)
{
  // The characters PHP's trim() removes.
  const std::string space(" \t\n\r\v\0", 6);
  size_t begin = email.find_first_not_of(space);
  size_t end = email.find_last_not_of(space);

  // 64-bit FNV-1a of the lowercased email.
  seed_t hash = 0xCBF29CE484222325ull;
  for (size_t i = begin; begin != std::string::npos && i <= end; ++i) {
    unsigned char c = email[i];
    if (c >= 'A' && c <= 'Z') {
      c += 'a' - 'A';
    }
    hash ^= c;
    hash *= 0x100000001B3ull;
  }
  return hash;
}

static bool is_hidden(const fs::path &path)
{
  return path.filename().string().rfind('.', 0) == 0;
}

strvec_t utils_walkdir(filepath_t path)
{
  strvec_t file_paths;
  if (fs::is_regular_file(path)) {
    file_paths.push_back(path);
    return file_paths;
  }

  for (const auto &entry : fs::directory_iterator(path)) {
    if (is_hidden(entry.path())) {
      continue;
    }
    if (entry.is_directory()) {
      strvec_t sub_dir = utils_walkdir(entry.path().string());
      file_paths.insert(file_paths.end(), sub_dir.begin(), sub_dir.end());
    } else if (entry.is_regular_file()) {
      file_paths.push_back(entry.path().string());
    }
  }
  std::sort(file_paths.begin(), file_paths.end());
  return file_paths;
}

std::vector<ZipEntry> utils_zip_entries(filepath_t dir)
{
  std::vector<ZipEntry> entries;
  for (const std::string &file : utils_walkdir(dir)) {
    entries.push_back({file, fs::path(file).lexically_relative(dir).generic_string(), false});
  }
  return entries;
}

void utils_zip_files(filepath_t out_file_name, const std::vector<ZipEntry> &entries, const std::string &password)
{
  int error = 0;
  zip_t *archive = zip_open(out_file_name.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);
  if (!archive) {
    zip_error_t zip_error;
    zip_error_init_with_code(&zip_error, error);
    std::string message = "could not create " + out_file_name + ": " + zip_error_strerror(&zip_error);
    zip_error_fini(&zip_error);
    throw std::runtime_error(message);
  }

  auto fail = [&](const std::string &what) {
    std::string message = what + ": " + zip_strerror(archive);
    zip_discard(archive);
    throw std::runtime_error(message);
  };

  for (const ZipEntry &entry : entries) {
    if (!fs::is_regular_file(entry.path)) {
      zip_discard(archive);
      throw std::runtime_error("could not zip " + entry.path + ": no such file");
    }
    zip_source_t *source = zip_source_file(archive, entry.path.c_str(), 0, 0);
    if (!source) {
      fail("could not read " + entry.path);
    }
    zip_int64_t index = zip_file_add(archive, entry.name.c_str(), source, ZIP_FL_ENC_UTF_8);
    if (index < 0) {
      zip_source_free(source);
      fail("could not add " + entry.name + " to " + out_file_name);
    }
    if (entry.encrypted && zip_file_set_encryption(archive, index, ZIP_EM_AES_256, password.c_str()) < 0) {
      fail("could not encrypt " + entry.name + " in " + out_file_name);
    }
  }

  if (zip_close(archive) < 0) {
    fail("could not write " + out_file_name);
  }
}

std::string utils_file_to_str(filepath_t filepath)
{
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("could not read " + filepath);
  }
  std::stringstream buf;
  buf << file.rdbuf();
  return buf.str();
}

std::string utils_html_printf(const std::string &title, filepath_t desc_filepath, const strvec_t &args,
                              const std::string &extra_head)
{
  const std::string delim = "%DELIM";
  const std::string desc = utils_file_to_str(desc_filepath);
  const std::string arg_count = std::to_string(args.size()) + " argument" + (args.size() == 1 ? "" : "s");

  std::string body;
  size_t used = 0;
  size_t pos = 0;
  for (size_t found; (found = desc.find(delim, pos)) != std::string::npos; pos = found + delim.size()) {
    if (used == args.size()) {
      throw std::runtime_error(desc_filepath + " has more %DELIMs than the " + arg_count + " given");
    }
    body.append(desc, pos, found - pos);
    body += args[used++];
  }
  body.append(desc, pos, std::string::npos);
  if (used != args.size()) {
    throw std::runtime_error(desc_filepath + " has " + std::to_string(used) + " %DELIMs, but " + arg_count + " were given");
  }

  // The body is wrapped in a <section>: not a <p>, because descriptions contain block elements,
  // and not a <div>, because several descriptions style `.container div`.
  return utils_file_to_str("../resources/templates/header.txt")
    + extra_head
    + "<h2 style=\"text-align:center\">" + title + "</h2>\n"
    + "<section style=\"text-align:center\">\n"
    + body
    + "\n</section>"
    + utils_file_to_str("../resources/templates/footer.txt");
}
