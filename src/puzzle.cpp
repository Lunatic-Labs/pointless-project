#include <iostream>
#include <string>
#include <vector>

#include "./include/utils.h"
#include "./include/puzzle.h"

#define BOLD(s) "<b>" + (s) + "</b>"

Puzzle puzzle_create1(long seed)
{
  // TODO: Change in final version to bigger numbers.
  int a = utils_rng_roll(1, 5, seed);
  int b = utils_rng_roll(6, 9, seed);
  int s = a+b;
  std::string description = utils_file_to_str("./puzzle1/.desc.txt");
  std::string question = BOLD("what is " + std::to_string(a) + "+" + std::to_string(b) + "?");
  description.append(question);
  
  std::string html_content = utils_generate_html("Hello Pointless", description, seed);

  utils_generate_file("./puzzle1/instructions.html", html_content);
  return Puzzle{"puzzle1", std::to_string(s)};
}

Puzzle puzzle_create2(long seed)
{
  std::string description = utils_file_to_str("./puzzle2/.desc.txt");
  std::string html_content = utils_generate_html("Color Puzzle", description, seed);
  utils_generate_file("./puzzle2/instructions.html", html_content);
  return Puzzle{"puzzle2", "331E54F4AA00"};
}

Puzzle puzzle_create3(long seed)
{
  std::string description = utils_file_to_str("./puzzle3/.desc.txt");
  std::string html_content = utils_generate_html("Maze Puzzle", description, seed);
  utils_generate_file("./puzzle3/instructions.html", html_content);
  return Puzzle{"puzzle3", "uru6r3u3ld"};
}

Puzzle puzzle_create4(long seed)
{
  (void)seed;
  return Puzzle{"puzzle4", ""};
}
