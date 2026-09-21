/*
 * File: rematch-based-puzzle.cpp
 * Author: Mekeal Brown
 * Date: 2/6/2024
 * Description:
 *   The user is told how an alien number system writes numbers in three bases and is shown a
 *   table of values, three per row. For each row, they convert the values to base 10, sum them,
 *   and take the sum's least significant bit. The bits, first row first, make a binary number;
 *   the password is that number in base 10.
 */

#include <string>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ROWS 8 // Must match the table in files-rematch-based/.desc.html
#define VALUES_PER_ROW 3

// A value in the table and the base it is written in.
struct Value {
  std::string text;
  int base;
};

// Returns a random value, written by the rules in files-rematch-based/.desc.html:
// 0 through 20 are base 20, 21 through 42 are base 31, and A through F are base 16.
static Value random_value(seed_t &seed)
{
  int n = utils_rng_roll(0, 67, seed);
  if (n <= 20) {
    return {std::to_string(n), 20};
  }
  if (n <= 42) {
    return {std::to_string(n), 31};
  }
  return {std::string(1, (char)('A' + utils_rng_roll(0, 5, seed))), 16};
}

Puzzle rematch_based_puzzle_create(seed_t seed)
{
  strvec_t values;
  int answer = 0;
  for (int row = 0; row < ROWS; row++) {
    int sum = 0;
    for (int i = 0; i < VALUES_PER_ROW; i++) {
      Value value = random_value(seed);
      values.push_back(value.text);
      sum += std::stoi(value.text, nullptr, value.base);
    }
    answer = (answer << 1) | (sum & 1);
  }

  std::string html_content = utils_html_printf("../resources/files-rematch-based/.desc.html", values);
  utils_generate_file("../resources/files-rematch-based/instructions.html", html_content);
  return {"../resources/files-rematch-based", html_content, std::to_string(answer), "", {}};
}
