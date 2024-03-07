#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle color_puzzle_create(long seed)
{
  int rand = utils_rng_roll(0, 1, seed);
  std::string color_hex = rand ? "331E54" : "F4AA00";
  std::string color_name = rand ? "Purple" : "Gold";

  std::string html = utils_html_printf("Color Puzzle", "./files-color/.desc.txt", {color_hex, color_name});
  utils_generate_file("./files-color/instructions.html", html);

  return {"files-color", color_hex, {}};
}