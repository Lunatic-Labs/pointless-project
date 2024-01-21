#include "./include/html-generator.h"
#include <sys/stat.h>

std::string generate_html(std::string title, std::string description, long seed)
{
  // Stores the content of template_header into a std::string.
  char c;
  std::string header_content;
  std::string header_path = "./puzzles/resources/template-header.txt";

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

  std::string footer_path = "./puzzles/resources/template-footer.txt";

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

  // std::string file_name = title + ".html";

  // Creates and inserts content into a new file with the title as name.html
  // std::ofstream outfile(file_name);
  // outfile << content_concatenated;
  // outfile << template_seed;
  // outfile.close();

  // return file_name;
};

