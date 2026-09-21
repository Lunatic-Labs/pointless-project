#include <cstring>
#include <exception>
#include <iostream>

#include "./include/test.h"

uint32_t FLAGS = 0;

// Runs the tests whose names contain argv[1] (every test if there is none).
// Exits with status 1 if any test fails.
int main(int argc, char **argv)
{
  FLAGS |= ANS_ONLY;

  const struct {
    const char *name;
    void (*run)(void);
  } tests[] = {
    {"utils", utils_test},
    {"inventory", inventory_test},
    {"math_puzzle", math_puzzle_test},
    {"color_puzzle", color_puzzle_test},
    {"pixel_puzzle", pixel_puzzle_test},
    {"maze_puzzle", maze_puzzle_test},
    {"based_intro_puzzle", based_intro_puzzle_test},
    {"encrypt_puzzle", encrypt_puzzle_test},
    {"rematch_maze_puzzle", rematch_maze_puzzle_test},
    {"rematch_encrypt_puzzle", rematch_encrypt_puzzle_test},
    {"rematch_based_puzzle", rematch_based_puzzle_test},
    {"rematch_puzzle", rematch_puzzle_test},
    {"binary_addition_puzzle", binary_addition_puzzle_test},
    {"logicgate_puzzle", logicgate_puzzle_test},
    {"bst_puzzle", bst_puzzle_test},
    {"fin_puzzle", fin_puzzle_test},
    {"seeds", seeds_test},
    {"game_zipfiles", game_zipfiles_test},
  };

  const char *filter = argc > 1 ? argv[1] : "";
  int run = 0;
  int failed = 0;
  for (const auto &test : tests) {
    if (!std::strstr(test.name, filter)) {
      continue;
    }
    int before = test_failures();
    try {
      test.run();
    } catch (const std::exception &e) {
      test_check(false, std::string("unexpected exception: ") + e.what(), test.name, 0);
    }
    bool ok = test_failures() == before;
    std::cout << (ok ? "PASS " : "FAIL ") << test.name << std::endl;
    run++;
    failed += !ok;
  }

  std::cout << "\n" << run - failed << " passed, " << failed << " failed" << std::endl;
  if (run == 0) {
    std::cout << "no tests match '" << filter << "'" << std::endl;
    return 1;
  }
  return failed ? 1 : 0;
}
