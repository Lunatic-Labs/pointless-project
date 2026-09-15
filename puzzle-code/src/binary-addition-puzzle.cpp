/*
 * File: binary-addition-puzzle.cpp
 * Date: 10/2/2024
 * Description:
 *   Graph Paper Robot I. The user follows the robot's rules on a grid holding two random 7-bit
 *   numbers (red squares are 0, green are 1), which adds them. The password is the 8-bit sum.
 */

#include <bitset>
#include <string>

#include "./include/graphics.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define TAPE_WIDTH 7  // Bits in each number
#define TAPE_HEIGHT 3 // Grid rows: the two numbers and a blank row
#define REQ_CARRIES 2 // Minimum number of columns where both numbers have a 1

static constexpr Pixel EMPTY{255, 255, 255, 0};
static constexpr Pixel RED{255, 0, 0};      // 0
static constexpr Pixel GREEN{0, 255, 0};    // 1
static constexpr Pixel GOLD{255, 255, 0};   // Where the robot starts
static constexpr Pixel PURPLE{255, 0, 255}; // Where it stops

// Returns TAPE_WIDTH bits in a random order, TAPE_WIDTH / 2 of them 1.
static std::string random_bits(seed_t &seed)
{
  std::string bits(TAPE_WIDTH / 2, '1');
  bits.resize(TAPE_WIDTH, '0');
  utils_shuffle(bits, seed);
  return bits;
}

Puzzle binary_addition_puzzle_create(seed_t seed)
{
  std::string top;
  std::string bottom;
  int carries = 0;
  while (carries < REQ_CARRIES) {
    top = random_bits(seed);
    bottom = random_bits(seed);
    carries = 0;
    for (size_t i = 0; i < TAPE_WIDTH; ++i) {
      if (top[i] == '1' && bottom[i] == '1') {
        ++carries;
      }
    }
  }

  std::string sum = std::bitset<TAPE_WIDTH + 1>(std::stoi(top, nullptr, 2) + std::stoi(bottom, nullptr, 2)).to_string();

  Image graph(TAPE_WIDTH + 2, TAPE_HEIGHT);
  for (Pixel &p : graph.pixels) {
    p = EMPTY;
  }
  graph(0, 0) = PURPLE;
  graph(0, TAPE_WIDTH + 1) = GOLD;
  for (size_t i = 0; i < TAPE_WIDTH; ++i) {
    graph(0, i + 1) = top[i] == '1' ? GREEN : RED;
    graph(1, i + 1) = bottom[i] == '1' ? GREEN : RED;
  }

  std::string svg_html = graphics_gen_svg_from_image(graph, 20, {}).build(true);
  std::string html_body = utils_html_printf("Graph Paper Robot", "../resources/files-binary-addition/.desc.txt", {svg_html});
  utils_generate_file("../resources/files-binary-addition/instructions.html", html_body);
  return {"../resources/files-binary-addition", html_body, sum, {}};
}
