#ifndef PUZZLE_H
#define PUZZLE_H

#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

struct Puzzle {
  // The filepath to the appropriate directory
  // that contains all of the information needed
  // for the puzzle.
  std::string contents_fp;

  std::string password;

  std::optional<std::string> extra_info;
};

Puzzle math_puzzle_create(long seed);
Puzzle color_puzzle_create(long seed);
Puzzle pixel_puzzle_create(long seed);
Puzzle maze_puzzle_create(long seed);
Puzzle encrypt_puzzle_create(long seed);
Puzzle based_puzzle_create(long seed);
Puzzle ast_puzzle_create(long seed);
Puzzle fin_puzzle_create(long seed);

#endif // PUZZLE_H
