#include "./include/utils.h"
#include "./include/color-puzzle.h"

Puzzle color_puzzle_create(long seed)
{
  std::string description = utils_file_to_str("./files-color/.desc.txt");
  std::string html_content = utils_generate_html("Color Puzzle", description, seed);
  utils_generate_file("./files-color/instructions.html", html_content);
  return Puzzle{"files-color", "331E54F4AA00"};
}