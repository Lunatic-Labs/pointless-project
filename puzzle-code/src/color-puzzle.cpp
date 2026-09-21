/*
 * File: color-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Date: 1/27/2024
 * Description:
 *   The user is presented with either the color purple or gold (Lipscomb's colors).
 *   They are expected to find Lipscomb's hex color code for the color that is chosen.
 *   The password is the hex color code.
 */

#include "./include/inventory.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle color_puzzle_create(seed_t seed)
{
  int rand = utils_rng_roll(0, 1, seed);
  std::string color_hex = rand ? "331E54" : "F4AA00";
  std::string color_name = rand ? "Purple" : "Gold";

  const std::string token = utils_token(utils_derive_seed(seed, "token"));
  std::string html = utils_html_printf("../resources/files-color/.desc.html", {color_hex, color_name}, token,
                                       inventory_html("color"));
  utils_generate_file("../resources/files-color/instructions.html", html);
  return {"../resources/files-color", html, color_hex, token, {}};
}
