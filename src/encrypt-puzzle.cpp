#include <algorithm>
#include <iostream>
#include <cassert>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ENCR_WORDS {       \
  "lipscomb university",   \
  "computer science",      \
  "programming languages", \
  "data structures",       \
  "dijkstras algorithm",   \
  "depth first search",    \
  "breadth first search",  \
  "graph theory",          \
  "dynamic programming",   \
  "greedy algorithms",     \
  "big o notation",        \
  "recursive function",    \
  "binary search tree",    \
}

#define ENCR_OPS_SHIFT 0
#define ENCR_OPS_SWAP 1
#define ENCR_OPS_CHANGE 2

#define ENCR_OPS {  \
  ENCR_OPS_CHANGE,   \
  ENCR_OPS_SWAP,     \
  ENCR_OPS_SHIFT     \
}

#define ENCR_ROTATIONS_MIN 1 // Must be 1 or greater
#define ENCR_ROTATIONS_MAX 3

#define ENCR_CHANGE_MIN 1 // Must be 1 or greater
#define ENCR_CHANGE_MAX 3

void shift(std::string &s, int dir, int n)
{
  if (dir) {
    std::rotate(s.rbegin(), s.rbegin() + n, s.rend());
  } else {
    std::rotate(s.begin(), s.begin() + n, s.end());
  }
}

void swap(std::string &s, int a, int b)
{
  std::swap(s[a], s[b]);
}

void change(std::string &s, int n)
{
  assert(n > 0 && n < 27 && "Invalid change");
  for (size_t i = 0, c = 0; i < s.size(); ++i) {
    if (isalpha(s[i])) {
      ++c;
    }
    if (c == 4) {
      s[i] -= 'a';
      s[i] = (s[i] + n) % 26;
      s[i] += 'a';
      c = 0;
    }
  }
}

std::string num_to_english(int idx) 
{
  std::string numstr = std::to_string(idx);
  if (idx%10 == 1) {
    return numstr + "st";
  }
  if (idx%10 == 2) {
    return numstr  + "nd";
  }
  return numstr + "th";
}

Puzzle encrypt_puzzle_create(long seed)
{
  std::string words[] = ENCR_WORDS;
  const int word_idx = utils_rng_roll(0, sizeof(words)/sizeof(*words)-1, seed);

  std::string word = words[word_idx];
  std::vector<int> ops = ENCR_OPS;

  std::vector<std::string> instructions;

  for (int i = 0; ops.size(); ++i) {
    int rng = utils_rng_roll(0, ops.size()-1, seed);
    size_t len = word.size()-1;
    switch (ops[rng]) {
      case 0: {
        int dir = utils_rng_roll(0, 1, seed);
        int rotations = utils_rng_roll(ENCR_ROTATIONS_MIN, ENCR_ROTATIONS_MAX, seed);
        instructions.push_back(std::to_string(i+1) + ".) Shifted all characters by " + std::to_string(rotations) + " to the " + (dir ? "right" : "left") + ".\n");
        shift(word, dir, rotations);
      } break;
      case 1: {
        int idx1 = utils_rng_roll(0, len, seed);
        int idx2 = utils_rng_roll(0, len, seed);
        instructions.push_back(std::to_string(i+1) + ".) Swapped the " + num_to_english(idx1+1) + " and " + num_to_english(idx2+1) + " characters.\n");
        swap(word, idx1, idx2);
      } break;
      case 2: {
        int jump_dist = utils_rng_roll(ENCR_CHANGE_MIN, ENCR_CHANGE_MAX, seed);
        instructions.push_back(std::to_string(i+1) + ".) Alphabetically increased every 4th character by " + std::to_string(jump_dist) + ".\n");
        change(word, jump_dist);
      } break;
      default:
        assert(false && "Invalid operation");
    }
    ops.erase(ops.begin() + rng);
  }
  instructions.insert(instructions.begin(), word);

  Puzzle p{"files-encrypt", words[word_idx]};

  if (FLAGS & ANS_ONLY) {
    return p;
  }

  std::string html_body = utils_html_printf("Encrypt", "./files-encrypt/.desc.txt", instructions);
  utils_generate_file("./files-encrypt/instructions.html", html_body);

  return p;
}