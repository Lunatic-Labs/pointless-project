#include <cassert>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <zip.h>

void zip_files(std::string out_file_name,
              std::vector<std::string> file_names,
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

    zip_source_t * src = zip_source_file(zip_file, file_names[i].c_str(), 0, 0);
    zip_add(zip_file, file_names[i].c_str(), src);

    if (!i && !password.empty()) {
      zip_file_set_encryption(zip_file, i, ZIP_EM_AES_256, password.c_str());
    }
  }
  zip_close(zip_file);
}
 
   