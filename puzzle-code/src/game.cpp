#include <cstdio>

#include "./include/game.h"

std::vector<Puzzle> game_create_puzzles(seed_t seed)
{
  // Each puzzle gets its own seed, so that no two puzzles share random numbers.
  // Braced lists are evaluated in order, so the puzzles are created in play order.
  return {
    math_puzzle_create(utils_derive_seed(seed, "math")),
    color_puzzle_create(utils_derive_seed(seed, "color")),
    pixel_puzzle_create(utils_derive_seed(seed, "pixel")),
    maze_puzzle_create(utils_derive_seed(seed, "maze")),
    based_intro_puzzle_create(utils_derive_seed(seed, "based-intro")),
    encrypt_puzzle_create(utils_derive_seed(seed, "encrypt")),
    rematch_puzzle_create(utils_derive_seed(seed, "rematch")),
    binary_addition_puzzle_create(utils_derive_seed(seed, "binary-addition")),
    logicgate_puzzle_create(utils_derive_seed(seed, "logicgate")),
    bst_puzzle_create(utils_derive_seed(seed, "bst")),
    fin_puzzle_create(utils_derive_seed(seed, "fin")),
  };
}

std::string game_puzzle_name(const Puzzle &puzzle)
{
  const std::string prefix = "files-";
  size_t found = puzzle.contents_fp.rfind(prefix);
  return found == std::string::npos ? puzzle.contents_fp : puzzle.contents_fp.substr(found + prefix.size());
}

void game_print_passwords(const std::vector<Puzzle> &puzzles)
{
  for (const Puzzle &puzzle : puzzles) {
    std::printf("%-15s Password: %s", game_puzzle_name(puzzle).c_str(), puzzle.password.c_str());
    if (puzzle.extra_info) {
      std::printf("  %s", puzzle.extra_info->c_str());
    }
    std::printf("\n");
  }
}

void game_write_zipfiles(const std::vector<Puzzle> &puzzles, const std::string &zipdir)
{
  utils_mkdir(zipdir);

  // Innermost first, since each zip goes inside the one before it.
  for (size_t n = puzzles.size(); n >= 1; --n) {
    const Puzzle &puzzle = puzzles[n - 1];
    std::vector<ZipEntry> entries = utils_zip_entries(puzzle.contents_fp);
    if (n < puzzles.size()) {
      const std::string next = "puzzle" + std::to_string(n + 1) + ".zip";
      entries.insert(entries.begin(), ZipEntry{zipdir + "/" + next, next, true});
    }
    utils_zip_files(zipdir + "/puzzle" + std::to_string(n) + ".zip", entries, puzzle.password);
  }
}
