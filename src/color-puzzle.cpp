#include "./include/utils.h"
#include "./include/color-puzzle.h"

Puzzle color_puzzle_create(long seed)
{
  std::string description = utils_file_to_str("./files-color/.desc.txt");
  int rand = utils_rng_roll(0, 1, seed);
  std::string color_hex = rand ? "331E54" : "F4AA00";
  std::string color_name = rand ? "Purple" : "Gold";

  description += color_name + "</b>";
  description += "<ul style=\"display: inline-block; float: right; margin-right: 50%\">"
                  "    <li>6-character sequence,</li>"
                  "    <li>hexadecimal color code,</li>"
                  "    <li>uppercase letters.</li>"
                  "</ul>";
  std::string html_content = utils_generate_html("Color Puzzle", description, seed);
  utils_generate_file("./files-color/instructions.html", html_content);

  return Puzzle{"files-color", color_hex};
}