#include <iostream>
#include <string>
#include <vector>

#include "./include/utils.h"
#include "./include/puzzle.h"

Puzzle puzzle_create1(long seed)
{
  int a = utils_rng_roll(1, 50, seed);
  int b = utils_rng_roll(51, 100, seed);
  int s = a+b;
  std::string instrs = "what is " + std::to_string(a) + "+" + std::to_string(b) + "?";

  generate_file("./puzzle1/instructions.txt", instrs);
  return Puzzle{"puzzle1", std::to_string(s)};
}

Puzzle puzzle_create2(long seed)
{
  (void)seed;
  return Puzzle{"puzzle2", "2"};
}

Puzzle puzzle_create3(long seed)
{
  (void)seed;
  return Puzzle{"puzzle3", "3"};
}

Puzzle puzzle_create4(long seed)
{
  (void)seed;
  return Puzzle{"puzzle4", ""};
}
