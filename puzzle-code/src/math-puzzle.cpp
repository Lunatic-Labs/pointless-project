/*
 * File: math-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Contact: zdhdev@yahoo.com, tcaustin@mail.lipscomb.edu
 * Date: 1/20/2024
 * Description:
 *   The user is presented with two numbers and is asked to add them together,
 *   with the password being the sum of the two numbers.
 */

#include "./include/puzzle.h"
#include "./include/utils.h"

#define MATH_MIN1 20
#define MATH_MAX1 39
#define MATH_MIN2 40
#define MATH_MAX2 90

Puzzle math_puzzle_create(long seed)
{
  int a = utils_rng_roll(MATH_MIN1, MATH_MAX1, seed);
  int b = utils_rng_roll(MATH_MIN2, MATH_MAX2, seed);
  int s = a+b;

  std::string html_content = utils_html_printf("Hello Pointless", "../html-txt/files-math/.desc.txt",
                                                {std::to_string(a), std::to_string(b)});
  utils_generate_file("../html-txt/files-math/instructions.html", html_content);
  return {"../html-txt/files-math", html_content, std::to_string(s), {}};
}
