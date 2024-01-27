#include <cassert>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <zip.h>

#include "./include/utils.h"

std::string utils_generate_html(std::string title, std::string description, long seed)
{
  // Stores the content of template_header into a std::string.
  char c;
  std::string header_content;
  std::string header_path = "./resources/template-header.txt";

  struct stat buf;
  if(stat(header_path.c_str(), &buf) != 0) {
    std::cerr << "Can not open " + header_path + ": No such file" << std::endl;
    throw("Can not open " + header_path + ": No such file");
  }

  std::ifstream header_file(header_path);

  header_file >> std::noskipws;
  while ( header_file >> c ) {
    header_content += c;
  }

  header_file.close();

  // Stores the content of template_footer into a std::string.
  char d;
  std::string footer_content;

  std::string footer_path = "./resources/template-footer.txt";

  if(stat(footer_path.c_str(), &buf) != 0) {
    throw("Can not open " + footer_path + ": No such file");
  }

  std::ifstream footer_file(footer_path);
  footer_file >> std::noskipws;
  while (footer_file >> d) {
    footer_content += d;
  }
  footer_file.close();

  std::string content_concatenated = header_content
    + "<h3 style=\"text-align:center\">"
    + title
    + "</h3>"
    + "<p style=\"text-align:center\">"
    + description
    + "</p>"
    + footer_content;

  std::string template_seed = std::to_string(seed)
    + "</div>\n"
    + "</div>\n"
    + "</div>\n"
    + "</body>\n"
    + "</html>\n";

  return content_concatenated;
}

std::string utils_file_to_str(const std::string filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filepath << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::stringstream buf;
  buf << file.rdbuf();
  std::string contents = buf.str();
  file.close();
  return contents;
}

void utils_generate_file(std::string output_filepath, std::string output_body)
{
  std::ofstream outfp(output_filepath);
  outfp << output_body;
  outfp.close();
}

int utils_rng_roll(int min, int max, long seed)
{
  srand(seed);

  assert(min != max);
  if(min > max) {
    return utils_rng_roll(max, min, seed);
  }

  return rand() % (max - min + 1) + min;
}

int utils_roll_seed(void)
{
  long seed = (uint)time(nullptr);
  srand(seed);
  return seed;
}

// Description:
//   Zips a vector of files into a single zip file.
// Parameters:
//   `out_file_name` - The name of the zip file to create.
//   `file_names` - A vector of strings containing the paths to the files to zip.
//   `password` - The password to encrypt the _FIRST_ file with.
// Returns:
//   Nothing.
void utils_zip_files(std::string out_file_name,
                     strvec_t file_names,
                     std::string password)
{
  int *err = nullptr;
  zip *zip_file = zip_open(out_file_name.c_str(), ZIP_CREATE, nullptr);
  assert(!err);

  for (size_t i = 0; i < file_names.size(); ++i) {

    struct stat buffer;

    // File does not exist
    if (stat(file_names[i].c_str(), &buffer) != 0) {
      throw(std::runtime_error("Could not open file: " + file_names[i] + " File does not exist"));
    }

    zip_source_t *src = zip_source_file(zip_file, file_names[i].c_str(), 0, 0);

    // Strip the path from the file name (only the top most path).
    std::string::size_type pos = file_names[i].find('/');
    std::string stripped_filename = file_names[i].substr(pos + 1);

    zip_add(zip_file, stripped_filename.c_str(), src);

    if (!i && !password.empty()) {
      zip_file_set_encryption(zip_file, i, ZIP_EM_AES_256, password.c_str());
    }
  }
  zip_close(zip_file);
}

// Description:
//   Recursively walks a directory and returns a vector of all the files in the directory.
// Parameters:
//  path - The path to the directory to walk.
// Returns:
//  A vector of strings containing the paths to all the files in the directory.
strvec_t utils_walkdir(const std::string &path)
{
  strvec_t file_paths;

  if (std::filesystem::is_regular_file(path)) {
    file_paths.push_back(path);
    return file_paths;
  }

  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry.path()) && entry.path().filename().string()[0] != '.') {
      file_paths.push_back(entry.path());
    } else if (std::filesystem::is_directory(entry.path()) && entry.path().filename().string()[0] != '.') {
      strvec_t sub_dir = utils_walkdir(entry.path());
      file_paths.insert(file_paths.end(), sub_dir.begin(), sub_dir.end());
    }
  }

  return file_paths;
}

