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

void generate_file(std::string output_filepath, std::string output_body)
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

  return rand() % (max - min) + min;
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
    if (std::filesystem::is_regular_file(entry.path())) {
      file_paths.push_back(entry.path());
    } else if (std::filesystem::is_directory(entry.path())) {
      strvec_t sub_dir = utils_walkdir(entry.path());
      file_paths.insert(file_paths.end(), sub_dir.begin(), sub_dir.end());
    }
  }

  return file_paths;
}

