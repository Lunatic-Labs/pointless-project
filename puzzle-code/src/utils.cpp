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

const std::string TOKEN_ALPHABET = "ACDEFHJKLMNPRTVWXY3479";

std::string utils_token(seed_t seed)
{
  std::string token;
  for (int i = 0; i < TOKEN_LENGTH; i++) {
    token += TOKEN_ALPHABET[utils_rng_roll(0, (int)TOKEN_ALPHABET.size() - 1, seed)];
  }
  return token;
}

seed_t utils_roll_seed(void)
{
  std::random_device device;
  return ((seed_t)device() << 32) ^ device() ^ (seed_t)std::time(nullptr);
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
    if (entry.encrypted && zip_file_set_encryption(archive, index, ZIP_EM_TRAD_PKWARE, password.c_str()) < 0) {
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

// Escapes `text` for a place that holds text rather than markup, such as the <title> element.
static std::string escape_text(const std::string &text)
{
  std::string escaped;
  for (const char c : text) {
    switch (c) {
      case '&': escaped += "&amp;"; break;
      case '<': escaped += "&lt;"; break;
      case '>': escaped += "&gt;"; break;
      default: escaped += c;
    }
  }
  return escaped;
}

std::string utils_html_printf(filepath_t desc_filepath, const strvec_t &args, const std::string &token,
                              const std::string &inventory)
{
  const std::string param = "%PARAM";
  const std::string title_param = "%TITLE";
  const std::string title_directive = title_param + " ";
  const std::string file = utils_file_to_str(desc_filepath);

  // A page's words all live in its .desc.html, its title included: the first line is
  // "%TITLE <title>", and the rest of the file is the body.
  const size_t eol = file.find('\n');
  if (file.compare(0, title_directive.size(), title_directive) != 0 || eol == std::string::npos) {
    throw std::runtime_error(desc_filepath + " must start with a \"%TITLE <title>\" line");
  }
  const std::string title = file.substr(title_directive.size(), eol - title_directive.size());
  const std::string desc = file.substr(eol + 1);
  const std::string arg_count = std::to_string(args.size()) + " argument" + (args.size() == 1 ? "" : "s");

  std::string body;
  size_t used = 0;
  size_t pos = 0;
  for (size_t found; (found = desc.find(param, pos)) != std::string::npos; pos = found + param.size()) {
    if (used == args.size()) {
      throw std::runtime_error(desc_filepath + " has more %PARAMs than the " + arg_count + " given");
    }
    body.append(desc, pos, found - pos);
    body += args[used++];
  }
  body.append(desc, pos, std::string::npos);
  if (used != args.size()) {
    throw std::runtime_error(desc_filepath + " has " + std::to_string(used) + " %PARAMs, but " + arg_count + " were given");
  }

  // Every page that is a layer of its own carries the same sidebar, so it is built here rather
  // than in each .desc.html: the page's proof of progress at the top, the score (always 0, that
  // being the point) under it, and the inventory at the bottom. The first page has nothing in
  // hand yet, so its inventory is empty and the sidebar skips it. A page with no proof of
  // progress (the three rematch sub-puzzles) gets no sidebar at all. Styled by
  // `.container .sidebar` in resources/templates/header.html.
  const std::string inventory_block = inventory.empty() ? "" :
    "<div class=\"inventory\">\n" + inventory + "</div>\n";

  const std::string sidebar = token.empty() ? "" :
    "<div class=\"sidebar\">\n"
    "<div class=\"token\">\n"
    "<p><b>Proof of progress</b></p>\n"
    "<p><code>" + token + "</code></p>\n"
    "<p class=\"hint\">Type it into the Pointless website to record how far you have come.</p>\n"
    "</div>\n"
    "<p class=\"points\">Points: 0</p>\n"
    + inventory_block +
    "</div>\n";

  // The page names itself in the header's <title>, so a player with several layers open can tell the
  // tabs apart. templates/header.html holds the wording around the %TITLE placeholder. Unlike the
  // <h2>, <title> holds text rather than markup, so the title is escaped for it.
  filepath_t header_filepath = "../resources/templates/header.html";
  std::string header = utils_file_to_str(header_filepath);
  const size_t placeholder = header.find(title_param);
  if (placeholder == std::string::npos) {
    throw std::runtime_error(header_filepath + " has no " + title_param + " placeholder for the page title");
  }
  header.replace(placeholder, title_param.size(), escape_text(title));

  // The body is wrapped in a <section>: not a <p>, because descriptions contain block elements,
  // and not a <div>, because several descriptions style `.container div`. The title goes inside
  // the <section> it heads, which is where a heading belongs. The <section> and the sidebar are
  // the two columns of `.layout`: the puzzle on the left, what the player has to show for it on
  // the right.
  return header
    + "<div class=\"layout\">\n"
    + "<section>\n"
    + "<h2>" + title + "</h2>\n"
    + body
    + "\n</section>\n"
    + sidebar
    + "</div>\n"
    + utils_file_to_str("../resources/templates/footer.html");
}
