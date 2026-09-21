/*
 * File: encrypt-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Date: 2/14/2024
 * Description:
 *   The user is presented with a word that has been encrypted using a series of operations.
 *   We provide them with the steps that were taken to encrypt the word and they must decrypt it
 *   by reversing the operations. The password is the original word.
 */

#include <algorithm>
#include <string>
#include <vector>

#include "./include/inventory.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define ENCR_WORDS {       \
  "lipscomb_university",   \
  "computer_science",      \
  "programming_languages", \
  "data_structures",       \
  "dijkstras_algorithm",   \
  "depth_first_search",    \
  "breadth_first_search",  \
  "graph_theory",          \
  "dynamic_programming",   \
  "greedy_algorithms",     \
  "big_o_notation",        \
  "recursive_function",    \
  "binary_search_tree",    \
}

enum class Op { Shift, Swap, Change };

// The operations used, once each, in a random order.
#define ENCR_OPS { \
  Op::Change,      \
  Op::Swap,        \
  Op::Swap,        \
  Op::Swap,        \
  Op::Shift,       \
}

#define ENCR_ROTATIONS_MIN 1 // Must be 1 or greater
#define ENCR_ROTATIONS_MAX 3

#define ENCR_CHANGE_MIN 1 // Must be from 1 to 25
#define ENCR_CHANGE_MAX 3

static void shift(std::string &s, bool right, int n)
{
  if (right) {
    std::rotate(s.rbegin(), s.rbegin() + n, s.rend());
  } else {
    std::rotate(s.begin(), s.begin() + n, s.end());
  }
}

// Moves every 4th letter (not counting other characters) `n` letters later in the alphabet.
static void change(std::string &s, int n)
{
  int letters = 0;
  for (char &c : s) {
    if (c >= 'a' && c <= 'z' && ++letters % 4 == 0) {
      c = 'a' + (c - 'a' + n) % 26;
    }
  }
}

// Returns 1st, 2nd, 3rd, 4th, ..., 11th, 12th, 13th, ..., 21st, ...
static std::string ordinal(int n)
{
  const char *suffix = "th";
  if (n % 100 < 11 || n % 100 > 13) {
    switch (n % 10) {
      case 1: suffix = "st"; break;
      case 2: suffix = "nd"; break;
      case 3: suffix = "rd"; break;
    }
  }
  return std::to_string(n) + suffix;
}

Puzzle encrypt_puzzle_create(seed_t seed)
{
  const std::vector<std::string> words = ENCR_WORDS;
  const std::string password = words[utils_rng_roll(0, (int)words.size() - 1, seed)];
  std::string word = password;
  const int last = (int)word.size() - 1;

  std::vector<Op> ops = ENCR_OPS;
  std::string steps;
  while (!ops.empty()) {
    int i = utils_rng_roll(0, (int)ops.size() - 1, seed);
    steps += "<li>";
    switch (ops[i]) {
      case Op::Shift: {
        bool right = utils_chance(50, seed);
        int rotations = utils_rng_roll(ENCR_ROTATIONS_MIN, ENCR_ROTATIONS_MAX, seed);
        steps += "Shifted all characters by " + std::to_string(rotations) + " to the " + (right ? "right" : "left") + ".";
        shift(word, right, rotations);
      } break;
      case Op::Swap: {
        int a = utils_rng_roll(0, last, seed);
        int b = utils_rng_roll(0, last - 1, seed);
        if (b >= a) {
          b++; // So that b != a
        }
        steps += "Swapped the " + ordinal(a + 1) + " and " + ordinal(b + 1) + " characters.";
        std::swap(word[a], word[b]);
      } break;
      case Op::Change: {
        int n = utils_rng_roll(ENCR_CHANGE_MIN, ENCR_CHANGE_MAX, seed);
        steps += "Alphabetically increased every 4th letter by " + std::to_string(n) + ", skipping the _ character.";
        change(word, n);
      } break;
    }
    steps += "</li>\n";
    ops.erase(ops.begin() + i);
  }

  const std::string token = utils_token(utils_derive_seed(seed, "token"));
  std::string html_body = utils_html_printf("../resources/files-encrypt/.desc.html", {word, steps}, token,
                                            inventory_html("encrypt"));
  utils_generate_file("../resources/files-encrypt/instructions.html", html_body);
  return {"../resources/files-encrypt", html_body, password, token, {}};
}
