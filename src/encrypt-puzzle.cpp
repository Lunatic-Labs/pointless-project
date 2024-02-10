#include <algorithm>
#include <iostream>
#include <cassert>

#include "./include/puzzle.h"
#include "./include/utils.h"

std::string words[] = {
  "lipscomb university",
  "computer science",
  "programming languages",
  "data structures",
  "dijkstras algorithm",
  "depth first search",
  "breadth first search",
  "graph theory",
  "dynamic programming",
  "greedy algorithms",
  "big o notation",
  "recursive function",
  "binary search tree",
};

#define WORDS_LEN (sizeof(words) / sizeof(words[0]))

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

Puzzle encrypt_puzzle_create(long seed)
{
  const int word_idx = utils_rng_roll(0, WORDS_LEN-1, seed);

  std::string word = words[word_idx];
  std::vector<int> ops = {2,1,0};

  std::vector<std::string> instructions;

  for (int i = 0; ops.size(); ++i) {
    int rng = utils_rng_roll(0, ops.size()-1, seed);
    size_t len = word.size()-1;
    switch (ops[rng]) {
      case 0: {
        int dir = utils_rng_roll(0, 1, seed);
        int rotations = utils_rng_roll(1, 3, seed);
        instructions.push_back(std::to_string(i+1) + ".) Shifted all characters by " + std::to_string(rotations) + " to the " + (dir ? "right" : "left") + ".\n");
        shift(word, dir, rotations);
      } break;
      case 1: {
        int idx1 = utils_rng_roll(0, len, seed);
        int idx2 = utils_rng_roll(0, len, seed);
        instructions.push_back(std::to_string(i+1) + ".) Swapped characters at positions " + std::to_string(idx1) + " and " + std::to_string(idx2) + ".\n");
        swap(word, idx1, idx2);
      } break;
      case 2: {
        int jump_dist = utils_rng_roll(1, 3, seed);
        instructions.push_back(std::to_string(i+1) + ".) Alphabetically increased every 4th character by " + std::to_string(jump_dist) + ".\n");
        change(word, jump_dist);
      } break;
      default:
        assert(false && "Invalid operation");
    }
    ops.erase(ops.begin() + rng);
  }
  instructions.insert(instructions.begin(), word);

  std::string html_body = utils_html_printf("Encrypt", "./files-encrypt/.desc.txt", instructions);
  utils_generate_file("./files-encrypt/instructions.html", html_body);

  return Puzzle{"files-encrypt", words[word_idx]};
}