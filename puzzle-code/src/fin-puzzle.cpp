#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle fin_puzzle_create(long seed)
{
  (void)seed;

  std::string html_content = utils_html_printf("Goodbye Pointless", "../html-txt/files-fin/.desc.txt", {});
  utils_generate_file("../html-txt/files-fin/instructions.html", html_content);
  return {"../html-txt/files-fin", html_content, "", {}};
}