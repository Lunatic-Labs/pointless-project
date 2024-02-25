#include <iostream>
#include <bitset>

#include "./include/utils.h"
#include "./include/puzzle.h"
#include "./include/graphics.h"

#define TAPE_WIDTH 6
#define TAPE_HEIGHT 3

#define EMPTY {255, 255, 255} // blank
#define RED {255, 0, 0}       // off
#define GREEN {0, 255, 0}     // on
#define GOLD {255, 255, 0}    // start
#define PURPLE {255, 0, 255}  // end

Puzzle binary_addition_puzzle_create(long seed)
{
  // TODO: make sure not 0 and not equal
  int top = utils_rng_roll(0, 31, seed);
  int bottom = utils_rng_roll(0, 31, seed);

  std::string top_bin = std::bitset<5>(top).to_string();
  std::string bottom_bin = std::bitset<5>(bottom).to_string();
  std::string sum_bin = std::bitset<6>(top + bottom).to_string();

  Image graph(TAPE_WIDTH, TAPE_HEIGHT);

  for (size_t i = 0; i < TAPE_HEIGHT; ++i) {
    for (size_t j = 0; j < TAPE_WIDTH; ++j) {
      graph(i,j) = EMPTY;
    }
  }

  for (size_t i = 0; i < TAPE_WIDTH; ++i) {
    if (top_bin[i] == '1') {
      graph(0,i) = GREEN;
    } else {
      graph(0,i) = RED;
    }
    if (bottom_bin[i] == '1') {
      graph(1,i) = GREEN;
    } else {
      graph(1,i) = RED;
    }
  }

  std::string svg = graphics_gen_svg(graph, 20);
  std::string html_body = utils_html_printf("Graph Paper Robot", "./files-binary-addition/.desc.txt", {{svg}});
  utils_generate_file("./files-binary-addition/instructions.html", html_body);

  return Puzzle{"files-binary-addition", sum_bin, {}};
}