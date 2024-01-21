#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>

struct Puzzle {
  std::string contents_fp;
  std::string password;
};

Puzzle construct_puzzle1(long seed);
Puzzle construct_puzzle2(long seed);
Puzzle construct_puzzle3(long seed);

#endif // PUZZLE_H
