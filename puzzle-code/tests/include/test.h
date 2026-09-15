#ifndef TEST_H
#define TEST_H

#include <exception>
#include <optional>
#include <string>
#include <vector>

#include "../../src/include/puzzle.h"
#include "../../src/include/utils.h"

// Records a failure, with its file and line, unless `cond` is true. The test keeps going.
#define CHECK(cond) test_check((cond), #cond, __FILE__, __LINE__)

// Like CHECK(actual == expected) for strings, but prints both when they differ.
#define CHECK_EQ(actual, expected) test_check_eq((actual), (expected), #actual, __FILE__, __LINE__)

// Records a failure unless `statement` throws a std::exception.
#define CHECK_THROWS(statement)                                               \
  do {                                                                        \
    bool thrown_ = false;                                                     \
    try { statement; } catch (const std::exception &) { thrown_ = true; }     \
    test_check(thrown_, "throws: " #statement, __FILE__, __LINE__);           \
  } while (0)

void test_check(bool ok, const std::string &what, const char *file, int line);
void test_check_eq(const std::string &actual, const std::string &expected, const std::string &what,
                   const char *file, int line);

// The number of failures so far.
int test_failures(void);

// The seed of an example player, test@example.com.
seed_t test_email_seed(void);

// What a puzzle should produce for a seed.
struct Expected {
  seed_t seed;
  std::string password;
  std::optional<std::string> extra_info = {}; // Checked if given
};

// For each of `expected`, checks the password (and extra_info) of create(seed), and that its page
// starts with the header, ends with the footer, and contains each of `snippets`.
// (Variadic, because the braced lists contain commas.)
#define CHECK_PUZZLE(create, ...) test_puzzle((create), __VA_ARGS__, __FILE__, __LINE__)
void test_puzzle(Puzzle (*create)(seed_t), const std::vector<Expected> &expected, const strvec_t &snippets,
                 const char *file, int line);

// Tests, listed in tests/main.cpp
void utils_test();
void math_puzzle_test();
void color_puzzle_test();
void pixel_puzzle_test();
void maze_puzzle_test();
void based_intro_puzzle_test();
void encrypt_puzzle_test();
void rematch_maze_puzzle_test();
void rematch_encrypt_puzzle_test();
void rematch_based_puzzle_test();
void rematch_puzzle_test();
void binary_addition_puzzle_test();
void logicgate_puzzle_test();
void bst_puzzle_test();
void fin_puzzle_test();
void seeds_test();
void game_zipfiles_test();

#endif // TEST_H
