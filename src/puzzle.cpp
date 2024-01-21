#include <iostream>
#include <string>
#include <vector>

#include "./include/puzzle.h"

Puzzle construct_puzzle1(long seed)
{
  (void)seed;
  return Puzzle{"puzzle1", "1"};
}

Puzzle construct_puzzle2(long seed)
{
  (void)seed;
  return Puzzle{"puzzle2", "2"};
}

Puzzle construct_puzzle3(long seed)
{
  (void)seed;
  return Puzzle{"puzzle3", "3"};
}

Puzzle construct_puzzle4(long seed)
{
  (void)seed;
  return Puzzle{"puzzle4", ""};
}