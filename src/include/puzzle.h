#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>

struct Puzzle {
  // The filepath to the appropriate directory
  // that contains all of the information needed
  // for the puzzle.
  std::string contents_fp;

  std::string password;
};

Puzzle puzzle_create1(long seed);
Puzzle puzzle_create2(long seed);
Puzzle puzzle_create3(long seed);
Puzzle puzzle_create4(long seed);

#endif // PUZZLE_H
