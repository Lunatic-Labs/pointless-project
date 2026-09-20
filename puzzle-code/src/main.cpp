#include <exception>
#include <iostream>
#include <string>

#include "./include/game.h"
#include "./include/utils.h"

uint32_t FLAGS = 0;

static const char *USAGE = "usage: ./main [-a] [-j] [-s <seed>]";

int main(int argc, char **argv)
{
  seed_t seed = 0;
  bool seed_given = false;
  bool json = false; // -j: print the answers as JSON instead of as lines for a person to read.

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "-a") {
      FLAGS |= ANS_ONLY;
    } else if (arg == "-j") {
      json = true;
    } else if (arg == "-s" && i + 1 < argc) {
      const std::string value = argv[++i];
      seed_given = true;
      size_t used = 0;
      try {
        seed = std::stoull(value, &used);
      } catch (const std::exception &) {
        used = 0;
      }
      if (used == 0 || used != value.size()) {
        std::cerr << "Expected a number after -s, got: " << value << std::endl;
        return 1;
      }
    } else {
      std::cerr << "Unknown or incomplete option: " << arg << "\n" << USAGE << std::endl;
      return 1;
    }
  }

  if (!seed_given) {
    seed = utils_roll_seed();
  }
  // Always printed, so a run with a random seed can be repeated. Under -j the seed is a field of
  // the JSON object instead, so that the whole of stdout parses as JSON.
  if (!json) {
    std::cout << "Seed: " << seed << std::endl;
  }

  try {
    std::vector<Puzzle> puzzles = game_create_puzzles(seed);
    if (json) {
      game_print_json(seed, puzzles);
    } else {
      game_print_passwords(puzzles);
    }
    if (!(FLAGS & ANS_ONLY)) {
      game_write_zipfiles(puzzles, "zipfiles");
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
