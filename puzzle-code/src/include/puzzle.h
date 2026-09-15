#ifndef PUZZLE_H
#define PUZZLE_H

#include <optional>
#include <string>

#include "./utils.h"

struct Puzzle {
  // The puzzle's resource directory, "../resources/files-<name>". Every file under it
  // (except names starting with `.`) goes in the puzzle's zip.
  std::string contents_fp;

  // The puzzle's instructions.html.
  std::string contents_html;

  // The answer, which unlocks the next puzzle's zip.
  std::string password;

  // Printed after the password for debugging; never shown to players.
  std::optional<std::string> extra_info;
};

// In play order (see game_create_puzzles()).
Puzzle math_puzzle_create(seed_t seed);
Puzzle color_puzzle_create(seed_t seed);
Puzzle pixel_puzzle_create(seed_t seed);
Puzzle maze_puzzle_create(seed_t seed);
Puzzle based_intro_puzzle_create(seed_t seed);
Puzzle encrypt_puzzle_create(seed_t seed);
Puzzle rematch_puzzle_create(seed_t seed);
Puzzle binary_addition_puzzle_create(seed_t seed);
Puzzle logicgate_puzzle_create(seed_t seed);
Puzzle bst_puzzle_create(seed_t seed);
Puzzle fin_puzzle_create(seed_t seed);

// The three parts of rematch_puzzle_create().
Puzzle rematch_maze_puzzle_create(seed_t seed);
Puzzle rematch_encrypt_puzzle_create(seed_t seed);
Puzzle rematch_based_puzzle_create(seed_t seed);

#endif // PUZZLE_H
