#include <iostream>
#include <bitset>
#include <cmath>
#include <random>
#include <algorithm>

#include "./include/utils.h"
#include "./include/puzzle.h"
#include "./include/graphics.h"

#define TAPE_WIDTH 7
#define TAPE_HEIGHT 3

#define REQ_CARRIES 2

#define EMPTY {255, 255, 255} // blank
#define RED {255, 0, 0}       // off
#define GREEN {0, 255, 0}     // on
#define GOLD {255, 255, 0}    // start
#define PURPLE {255, 0, 255}  // end

std::string create_bin_str(long seed)
{
  std::string bin;
  for(size_t i = 0; i < TAPE_WIDTH; ++i) {
    if (i < (TAPE_WIDTH/2)) {
      bin += '1';
    } else {
      bin += '0';
    }
  }
  std::shuffle(bin.begin(), bin.end(), std::default_random_engine(seed));
  return bin;
}

Puzzle binary_addition_puzzle_create(long seed)
{
  std::string top;
  std::string bottom;

  int carries = 0;
  while (carries < REQ_CARRIES) {
    top = create_bin_str(seed+=92);
    bottom = create_bin_str(seed+=2);
    carries = 0;
    for(size_t i = 0; i < TAPE_WIDTH; ++i) {
      if (top[i] == '1' && bottom[i] == '1') {
        ++carries;
      }
    }
  }

  std::string sum = std::bitset<TAPE_WIDTH+1>(stoi(top, nullptr, 2) + std::stoi(bottom, nullptr, 2)).to_string();

  Image graph(TAPE_WIDTH+2, TAPE_HEIGHT);

  for (size_t i = 0; i < TAPE_HEIGHT; ++i) {
    for (size_t j = 0; j < TAPE_WIDTH+2; ++j) {
      graph(i,j) = EMPTY;
    }
  }

  graph(0,0) = PURPLE;
  graph(0,TAPE_WIDTH+1) = GOLD;

  for (size_t i = 0; i < TAPE_WIDTH; ++i) {
    if (top[i] == '1') {
      graph(0,i+1) = GREEN;
    } else {
      graph(0,i+1) = RED;
    }
    if (bottom[i] == '1') {
      graph(1,i+1) = GREEN;
    } else {
      graph(1,i+1) = RED;
    }
  }

  // std::string svg = graphics_gen_svg(graph, 20);
  Svg svg = graphics_gen_svg_from_image(graph, 20.f);
  std::string svg_html = svg.build();
  std::string html_body = utils_html_printf("Graph Paper Robot", "./files-binary-addition/.desc.txt", {{svg_html}});
  utils_generate_file("./files-binary-addition/instructions.html", html_body);

  return Puzzle{"files-binary-addition", sum, {}};
}