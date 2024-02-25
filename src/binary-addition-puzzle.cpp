#include <iostream>
#include <bitset>
#include <cmath>

#include "./include/utils.h"
#include "./include/puzzle.h"
#include "./include/graphics.h"

#define TAPE_WIDTH 10
#define TAPE_HEIGHT 3

#define EMPTY {255, 255, 255} // blank
#define RED {255, 0, 0}       // off
#define GREEN {0, 255, 0}     // on
#define GOLD {255, 255, 0}    // start
#define PURPLE {255, 0, 255}  // end

Puzzle binary_addition_puzzle_create(long seed)
{
  // TODO: make sure not 0 and not equal
  int top = utils_rng_roll(0, std::pow(2, TAPE_WIDTH), seed);
  int bottom = utils_rng_roll(0, std::pow(2, TAPE_WIDTH), seed);
  std::cout << top << " " << bottom << std::endl;

  std::string top_bin = std::bitset<TAPE_WIDTH>(top).to_string();
  std::string bottom_bin = std::bitset<TAPE_WIDTH>(bottom).to_string();
  std::string sum_bin = std::bitset<TAPE_WIDTH+1>(top + bottom).to_string();

  Image graph(TAPE_WIDTH+2, TAPE_HEIGHT);

  for (size_t i = 0; i < TAPE_HEIGHT; ++i) {
    for (size_t j = 0; j < TAPE_WIDTH+2; ++j) {
      graph(i,j) = EMPTY;
    }
  }

  graph(0,0) = PURPLE;
  graph(0,TAPE_WIDTH+1) = GOLD;

  for (size_t i = 0; i < TAPE_WIDTH; ++i) {
    if (top_bin[i] == '1') {
      graph(0,i+1) = GREEN;
    } else {
      graph(0,i+1) = RED;
    }
    if (bottom_bin[i] == '1') {
      graph(1,i+1) = GREEN;
    } else {
      graph(1,i+1) = RED;
    }
  }

  std::string svg = graphics_gen_svg(graph, 20);
  std::string html_body = utils_html_printf("Graph Paper Robot", "./files-binary-addition/.desc.txt", {{svg}});
  utils_generate_file("./files-binary-addition/instructions.html", html_body);

  return Puzzle{"files-binary-addition", sum_bin, {}};
}