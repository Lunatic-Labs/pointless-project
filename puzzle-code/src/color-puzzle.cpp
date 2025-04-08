/*
 * File: color-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Contact: zdhdev@yahoo.com, tcaustin@mail.lipscomb.edu
 * Date: 1/27/2024
 * Description:
 *   The user is presented with either the color purple or gold (Lipscomb's colors).
 *   They are expected to find Lipscomb's hex color code for the color that is chosen.
 *   The password is the hex color code.
 */

#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle color_puzzle_create(long seed)
{
  int rand = utils_rng_roll(0, 1, seed);
  std::string color_hex = rand ? "331E54" : "F4AA00";
  std::string color_name = rand ? "Purple" : "Gold";

  std::string html = utils_html_printf("Color Puzzle", "../html-txt/files-color/.desc.txt", {color_hex, color_name});
  utils_generate_file("../html-txt/files-color/instructions.html", html);
  return {"../html-txt/files-color", html, color_hex, {}};
}
