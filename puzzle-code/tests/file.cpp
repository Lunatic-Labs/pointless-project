#include "file.h"

std::string file_contents(std::string file_dir) {
    std::string html_content;
    std::ifstream fp(file_dir);
    if (fp.is_open()) {
      std::stringstream buffer;
      buffer << fp.rdbuf();
      html_content = buffer.str();
      fp.close();
    } else {
      std::cerr << "Unable to open file header.txt" << std::endl;
    }

    return html_content;
}
