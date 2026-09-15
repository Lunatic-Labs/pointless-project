/*
 * File: rematch-encrypt-puzzle.cpp
 * Description:
 *   A harder encrypt puzzle. The page runs the password through three "machines" (JavaScript
 *   functions) and shows the result. The player can try each machine on their own input, work
 *   out what it does, and undo them. The password is the original word.
 */

#include <string>
#include <vector>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ENCR_WORDS { \
  "lipscomb",        \
  "computer",        \
  "languages",       \
  "theory",          \
  "program",         \
  "algorithm",       \
  "function",        \
  "procedure",       \
  "binary",          \
}

// Returns the entries of a JavaScript object mapping each character to another (a random
// one-to-one substitution), used by the Goopify machine.
static std::string substitution_table(seed_t &seed)
{
  const std::string from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{};':<>,./?`~";
  std::string to = from;
  utils_shuffle(to, seed);

  std::string table;
  for (size_t i = 0; i < from.size(); i++) {
    table += "  \"" + std::string(1, from[i]) + "\": \"" + std::string(1, to[i]) + "\",\n";
  }
  return table;
}

// Returns `s` as a JavaScript array of character codes, each XORed with `key`.
static std::string xored_codes(const std::string &s, int key)
{
  std::string res = "[";
  for (unsigned char c : s) {
    res += std::to_string(c ^ key) + ",";
  }
  return res + "]";
}

Puzzle rematch_encrypt_puzzle_create(seed_t seed)
{
  const std::vector<std::string> words = ENCR_WORDS;
  const std::string password = words[utils_rng_roll(0, (int)words.size() - 1, seed)];
  const std::string table = substitution_table(seed);
  const int key = utils_rng_roll(100, 250, seed);

  std::string html_body = utils_html_printf("Encrypt Rematch Puzzle", "../resources/files-rematch-encrypt/.desc.txt",
                                            {xored_codes(password, key), std::to_string(key), table});
  utils_generate_file("../resources/files-rematch-encrypt/instructions.html", html_body);
  return {"../resources/files-rematch-encrypt", html_body, password, {}};
}
