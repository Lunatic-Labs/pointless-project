#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

#include "./include/html-generator.h"
#include "./include/utils.h"
#include "./include/puzzle.h"

uint32_t FLAGS = 0;

void create_nested_zipfiles(std::vector<Puzzle> &puzzles)
{
  const char *zipdir = "zipfiles/puzzle";
  int i = puzzles.size();

  // Need to go in reverse order to zip the files correctly.
  for (auto puzzle = puzzles.rbegin(); puzzle != puzzles.rend(); ++puzzle, --i) {
    std::vector<std::string> files = utils_walkdir(puzzle->contents_fp);

    // Adds the next level of zipped puzzle (technically the previous one because reverse order).
    if(puzzle != puzzles.rbegin()) {
      files.insert(files.begin(), zipdir + std::to_string(i+1) + ".zip");
    }
    utils_zip_files(std::string(zipdir + std::to_string(i) + ".zip"), files, puzzle->password);
  }
}

#include "include/graphics.h"

int main(int argc, char **argv)
{
  long seed = 0;
  while (argc > 1) {
    if (strcmp(argv[1], "-a") == 0) {
      FLAGS |= ANS_ONLY;
    }
    else if (strcmp(argv[1], "-s") == 0) {
      FLAGS |= SET_SEED;
      --argc;
      ++argv;

      if (argc < 2 || (seed = atol(argv[1])) == 0) {
        std::cerr << "Expected <seed> (must be a number)" << std::endl;
        return 1;
      }
    }
    else {
      std::cerr << "Unknown flag: " << argv[1] << std::endl;
      return 1;
    }
    --argc;
    ++argv;
  }

  if ((FLAGS & SET_SEED) == 0) {
    seed = utils_roll_seed();
  }

  std::vector<Puzzle> puzzles = {
    math_puzzle_create(seed),
    color_puzzle_create(seed),
    pixel_puzzle_create(seed),
    maze_puzzle_create(seed),
    based_intro_puzzle_create(seed),
    encrypt_puzzle_create(seed),
    based_puzzle_create(seed),
    rematch_puzzle_create(seed),
    binary_addition_puzzle_create(seed),
    logicgate_puzzle_create(seed),
    ast_puzzle_create(seed),
    fin_puzzle_create(seed),
  };

  for (auto &puzzle : puzzles) {
    std::printf("%-15s Password: %s", puzzle.contents_fp.substr(6).c_str(), puzzle.password.c_str());
    if (puzzle.extra_info) {
      std::printf(" %s", puzzle.extra_info->c_str());
    }
    std::cout << std::endl;
  }

  if (!(FLAGS & ANS_ONLY)) {
    create_nested_zipfiles(puzzles);
  }

  return 0;
}
