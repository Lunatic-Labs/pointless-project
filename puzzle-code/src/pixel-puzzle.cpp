/*
* File: rematch-based-puzzle.cpp
* Author: Mekeal Brown
* Contact: brownmekeal@gmail.com
* Date: 2/6/2024
* Description:
*  The user is presented with three rows. One row has one hex value,
*  the second has two, and the third has three. The first two rows are solved.
*  The third requires the user to solve. The user must count the number of pixels
*  in the bison svg on the webpage and perform the arithmetic to solve.
*/

#include <unordered_map>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define MAX_LOOP 3 // if i can modify the html somehow...

static int get_pixel_count(const std::string& hexValue) {
    static std::unordered_map<std::string, int> pixelCounts = {
        {"#FFFFFF", 2},
        {"#000000", 30},
        {"#F4AA00", 6},
        {"#331E54", 66},
        {"#D2BB8D", 11},
        // {"#552D1B", 183} This is brown. We don't want them counting 183 pixels
    };

    auto it = pixelCounts.find(hexValue);
    if (it != pixelCounts.end()) {
        return it->second;
    }
    return 0; // not found?? not sure how that could happen
}

Puzzle pixel_puzzle_create(long seed)
{
  std::vector<std::string> hex_vals = {"#FFFFFF", "#000000", "#F4AA00", "#331E54", "#964B00"};
  std::vector<std::string> delim_values;

  for (int i = 0; i < MAX_LOOP; i++) {
    int prod = 1;
    for (int j = 0; j < i + 1; j++) {
      int rand = utils_rng_roll(0, hex_vals.size() - 1, seed);
      delim_values.push_back(hex_vals[rand]);
      prod *= get_pixel_count(hex_vals[rand]);
    }
    delim_values.push_back(std::to_string(prod));
  }
  std::string answer = delim_values.back();

  // Give the bison a grid
  FLAGS |= BISON_GRID;
  std::string html = utils_html_printf("Pixel Puzzle", "../html-txt/files-pixel/.desc.txt", {delim_values});

  // Disable it so later ones don't have it.
  FLAGS &= ~(BISON_GRID);

  utils_generate_file("../html-txt/files-pixel/instructions.html", html);
  return {"../html-txt/files-pixel", html, std::move(answer), {}};
}
