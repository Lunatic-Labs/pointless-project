#ifndef PUZZLE_H
#define PUZZLE_H

#include <optional>
#include <string>
#include <vector>

struct Puzzle {
  // The filepath to the appropriate directory
  // The content of the instruction.html pages
  // that contains all of the information needed
  // for the puzzle.
  std::string contents_fp;

  std::string contents_html;

  std::string password;

  std::optional<std::string> extra_info;
};

Puzzle math_puzzle_create(long seed);
Puzzle color_puzzle_create(long seed);
Puzzle pixel_puzzle_create(long seed);
Puzzle maze_puzzle_create(long seed);
Puzzle encrypt_puzzle_create(long seed);
Puzzle based_intro_puzzle_create(long seed);
Puzzle based_puzzle_create(long seed);
Puzzle bst_puzzle_create(long seed);
Puzzle binary_addition_puzzle_create(long seed);
Puzzle fin_puzzle_create(long seed);

Puzzle rematch_encrypt_puzzle_create(long seed);
Puzzle rematch_maze_puzzle_create(long seed);
Puzzle rematch_based_puzzle_create(long seed);
Puzzle rematch_puzzle_create(long seed);
Puzzle logicgate_puzzle_create(long seed);

#endif // PUZZLE_H
