#include "file.h"

std::string file_contents(std::string file_dir) {
    std::string html_content;
    std::ifstream header_file(file_dir);
    if (header_file.is_open()) {
      std::stringstream buffer;
      buffer << header_file.rdbuf();
      html_content = buffer.str();
      header_file.close();
    } else {
      std::cerr << "Unable to open file header.txt" << std::endl;
    }
  
    return html_content;
}
