#include <cassert>
#include <cstdlib>
#include <ctime>
#include <errno.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <zip.h>
#include <string.h>

#include "./include/utils.h"

int utils_chance(int percentage, long &seed)
{
  return utils_rng_roll(1, 100, seed) <= percentage; 
}

void utils_mkdir(filepath_t filepath)
{
  if (FLAGS & ANS_ONLY) {
    return;
  }
  if (std::filesystem::exists(filepath)) {
    return;
  }
  std::filesystem::create_directories(filepath);
}

std::string utils_html_printf(std::string title, filepath_t desc_filepath, strvec_t args)
{
  // Get the header content.
  char c;
  std::string header_content;
  std::string header_path = "../html-txt/resources/header.txt";

  struct stat buf;
  if(FLAGS & NO_HDR) {
    header_content = "";
  } else {
    if(stat(header_path.c_str(), &buf) != 0) {
      std::cerr << "Can not open " + header_path + ": No such file" << std::endl;
      throw("Can not open " + header_path + ": No such file");
    }

    std::ifstream header_file(header_path);

    header_file >> std::noskipws;
    while ( header_file >> c ) {
      header_content += c;
    }

    if (FLAGS & BISON_GRID) {
      header_content += "<style> svg{stroke:#000000;} </style>";
    }

    header_file.close();
  }

  // Get the footer content.
  char d;
  std::string footer_content;
  if(FLAGS & NO_FTR) {
    footer_content = "";
  } else {
    std::string footer_path = "../html-txt/resources/footer.txt";

    if (stat(footer_path.c_str(), &buf) != 0) {
      throw("Can not open " + footer_path + ": No such file");
    }

    std::ifstream footer_file(footer_path);
    footer_file >> std::noskipws;
    while (footer_file >> d) {
      footer_content += d;
    }
    footer_file.close();
  }

  // Get the description. Uses the `va_list` to get the variable arguments.
  std::string description = utils_file_to_str(desc_filepath);

  // std::stringstream ss(description);
  std::string desc_content = utils_file_to_str(desc_filepath);
  std::string body;

  const char *delim = "%DELIM";
  char *it = &desc_content[0];
  while (*it) {
    if (*it == '%' && (strncmp(it, delim, 6) == 0)) {
      if (args.empty()) {
        std::cerr << __FUNCTION__ << " Not enough arguments for description" << std::endl;
        std::exit(EXIT_FAILURE);
      }
      body += args.front();
      args.erase(args.begin());
      it += 5;
    } else {
      body += *it;
    }
    ++it;
  }

  //the additional tags are unsightly when using a custom header..ahem..based rematch
  std::string content_concatenated;
  if(FLAGS & NO_HDR) {
    content_concatenated = header_content
      + body
      + "</p>"
      + footer_content;
  } else {
      content_concatenated = header_content
      + "<h2 style=\"text-align:center\">"
      + title
      + "</h2>"
      + "<p style=\"text-align:center\">"
      + body
      + "</p>"
      + footer_content;
  }

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

void utils_generate_file(filepath_t output_filepath, std::string output_body)
{
  if (FLAGS & ANS_ONLY) {
    return;
  }
  std::ofstream outfp(output_filepath);
  if (!outfp.is_open()) {
    std::cerr << "Error opening file: " << output_filepath << std::endl;
    std::cerr << "reason: " << strerror(errno) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  outfp << output_body;
  outfp.close();
}

int utils_rng_roll(int min, int max, long &seed)
{
  srand(seed);
  seed += rand() % 1000;

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
void utils_zip_files(filepath_t out_file_name,
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

    // Strip the path from the file name
    size_t found = file_names[i].find("html-txt/");
    if (found != std::string::npos) {
      file_names[i].erase(file_names[i].begin(), file_names[i].begin()+12);
    }
    std::string::size_type pos = file_names[i].find("/");
    
    // Checking whether or not there is a dotfile after the first slash
    if(file_names[i][pos + 1] == '.') {
      // stripping that dotfile from the path
      pos = file_names[i].substr(pos + 1).find("/") + pos + 1;
    }
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
strvec_t utils_walkdir(filepath_t path)
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

