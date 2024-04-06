#include <cassert>
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <set>
#include <algorithm>

#include "./include/puzzle.h"
#include "./include/utils.h"
#include "./include/graphics.h"

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

std::string generate_sym_tbl(long seed)
{
  std::string from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{};':<>,./?`~";
  std::string to   = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{};':<>,./?`~";

  std::shuffle(to.begin(), to.end(), std::default_random_engine(seed));

  std::string tbl= "";
  for(size_t i = 0; i < to.length(); i++) {
    tbl.append("  \"");
    tbl.append(1, from[i]);
    tbl.append("\": \"");
    tbl.append(1, to[i]);
    tbl.append("\",");
    tbl += "\n";
  }

  return tbl;
}

std::string str_to_ascii_array(std::string &s, int key)
{
  std::string res = "[";
  // "{12, 43, 234, 654, 345}"
  for (auto c : s) {
    char s[8];
    sprintf(s, "%d", c^key);
    res += s;
    res += ",";
  }
  res += "]";

  return res;
}

Puzzle rematch_encrypt_puzzle_create(long seed)
{
  std::string words[] = ENCR_WORDS;
  const int word_idx = utils_rng_roll(0, sizeof(words)/sizeof(*words)-1, seed);

  std::string word = words[word_idx];

  std::string password = word;
  std::string len = "";

  std::string table = generate_sym_tbl(seed);

  int key = utils_rng_roll(100, 250, seed);

  std::string encrypted_password = str_to_ascii_array(password, key);
  std::string html_body = utils_html_printf("Encrypt puzzle", "./files-rematch-encrypt/.desc.txt", {encrypted_password, std::to_string(key), table});

  utils_generate_file("./files-rematch-encrypt/instructions.html", html_body);

  return {"files-rematch-encrypt", password, {}};
}
