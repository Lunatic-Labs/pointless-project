/*
 * File: pixel-puzzle.cpp
 * Author: Mekeal Brown
 * Contact: brownmekeal@gmail.com
 * Date: 2/6/2024
 * Description:
 *   The user is shown three rows of hex color codes: one code, then two, then three. The first
 *   two rows show how many pixels of each color the bison in the page's header has (multiplied
 *   together). The password is the product for the third row.
 */

#include <utility>
#include <vector>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ROWS 3 // Must match the rows in files-pixel/.desc.txt

// The colors of the bison in resources/templates/header.txt and how many pixels of each it has.
// Its 183 brown (#552D1B) pixels are left out so that nobody has to count them.
static const std::vector<std::pair<std::string, int>> BISON_COLORS = {
  {"#FFFFFF", 2},
  {"#000000", 30},
  {"#F4AA00", 6},
  {"#331E54", 66},
  {"#D2BB8D", 11},
};

Puzzle pixel_puzzle_create(seed_t seed)
{
  strvec_t delim_values;
  int product = 1;
  for (int row = 1; row <= ROWS; row++) {
    product = 1;
    for (int i = 0; i < row; i++) {
      const auto &[hex, count] = BISON_COLORS[utils_rng_roll(0, (int)BISON_COLORS.size() - 1, seed)];
      delim_values.push_back(hex);
      product *= count;
    }
    // The last row's product is the answer, so the page shows "?" instead.
    if (row < ROWS) {
      delim_values.push_back(std::to_string(product));
    }
  }

  // Outline the bison's pixels so that they can be counted.
  const std::string grid = "<style> svg{stroke:#000000;} </style>";
  std::string html = utils_html_printf("Pixel Puzzle", "../resources/files-pixel/.desc.txt", delim_values, grid);
  utils_generate_file("../resources/files-pixel/instructions.html", html);
  return {"../resources/files-pixel", html, std::to_string(product), {}};
}
