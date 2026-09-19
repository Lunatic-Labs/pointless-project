/*
 * File: bst-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin, Michael Hernandez-Lara
 * Date: 10/2/2024
 * Description:
 *   The user is given x and a binary tree of directories. Each directory has a "street sign" (a
 *   text file) with a condition on x: go right if it is true, left if not. The password is in
 *   ID.txt at the end of the correct path.
 */

#include <cstdio>
#include <string>
#include <vector>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define ROOT_MIN 11 // The range of x
#define ROOT_MAX 150
#define DEPTH 10    // Signs on the way from the top of the tree to an ID

namespace {

// A street sign: the name of its file, its condition, and whether that condition is true for x.
struct Sign {
  std::string name;
  std::string condition;
  bool right;
};

std::string str(int n)
{
  return std::to_string(n);
}

// Returns x if `right`, and otherwise some other number from 1 to ROOT_MAX.
int target(int x, bool right, seed_t &seed)
{
  if (right) {
    return x;
  }
  int n = utils_rng_roll(1, ROOT_MAX - 1, seed);
  return n >= x ? n + 1 : n;
}

// Returns a sign like "√((E - F) + (C * D)) = x".
Sign expression_sign(int x, bool right, seed_t &seed)
{
  const int v = target(x, right, seed);
  const bool root = utils_chance(50, seed);
  const int value = root ? v*v : v;

  // (left) +/- (c * d or c*d / d) = value
  const int c = utils_rng_roll(1, 12, seed);
  const int d = utils_rng_roll(1, 12, seed);
  const bool times = utils_chance(50, seed);
  const std::string term2 = times ? str(c) + " * " + str(d) : str(c*d) + " / " + str(d);
  const int term2_value = times ? c*d : c;
  const bool plus = utils_chance(50, seed) && value > term2_value;
  const int left = plus ? value - term2_value : value + term2_value;

  // left = e +/- f
  const int f = utils_rng_roll(1, 50, seed);
  const bool left_plus = utils_chance(50, seed) && left > f;
  const std::string term1 = left_plus ? str(left - f) + " + " + str(f) : str(left + f) + " - " + str(f);

  const std::string expression = "(" + term1 + ")" + (plus ? " + " : " - ") + "(" + term2 + ")";
  return {"Expression", (root ? "√(" + expression + ")" : expression) + " = x", right};
}

Sign make_sign(int x, seed_t &seed)
{
  const bool right = utils_chance(50, seed);
  switch (utils_rng_roll(1, 8, seed)) {
    case 1: {
      int limit = utils_rng_roll(ROOT_MIN - 1, ROOT_MAX - 1, seed);
      return {"Comparison", "x > " + str(limit), x > limit};
    }
    case 2: {
      int v = target(x, right, seed);
      int a = utils_rng_roll(1, v, seed);
      return {"Addition", str(a) + " + " + str(v - a) + " = x", right};
    }
    case 3: {
      int v = target(x, right, seed);
      int a = utils_rng_roll(1, 200, seed);
      return {"Subtraction", str(v + a) + " - " + str(a) + " = x", right};
    }
    case 4: {
      int v = target(x, right, seed);
      std::vector<int> divisors;
      for (int d = 1; d <= v; d++) {
        if (v % d == 0) {
          divisors.push_back(d);
        }
      }
      int a = divisors[utils_rng_roll(0, (int)divisors.size() - 1, seed)];
      return {"Multiplication", str(a) + " * " + str(v / a) + " = x", right};
    }
    case 5: {
      int v = target(x, right, seed);
      int a = utils_rng_roll(2, 12, seed);
      return {"Division", str(v*a) + " / " + str(a) + " = x", right};
    }
    case 6: {
      int v = target(x, right, seed);
      return {"Square_Root", "√" + str(v*v) + " = x", right};
    }
    case 7: {
      char hex[16];
      std::snprintf(hex, sizeof hex, "%X", target(x, right, seed));
      return {"Hexadecimal", std::string(hex) + " is x in hexadecimal", right};
    }
    default:
      return expression_sign(x, right, seed);
  }
}

struct Tree {
  int x;
  seed_t seed;
  std::string path; // The correct path: l or r for each sign
  int password = 0;
};

// Creates the directory `dir`. With `depth` 0 it gets an ID.txt; otherwise it gets a sign and
// left and right subdirectories. `on_path` says whether `dir` is on the correct path.
void build(Tree &tree, const std::string &dir, int depth, bool on_path)
{
  utils_mkdir(dir);
  if (depth == 0) {
    int id = utils_rng_roll(1000000, 9999999, tree.seed);
    utils_generate_file(dir + "/ID.txt", str(id));
    if (on_path) {
      tree.password = id;
    }
    return;
  }

  const Sign sign = make_sign(tree.x, tree.seed);
  utils_generate_file(dir + "/" + sign.name + ".txt", "if " + sign.condition + ", then go right\nelse, go left");
  if (on_path) {
    tree.path += sign.right ? 'r' : 'l';
  }
  build(tree, dir + "/left", depth - 1, on_path && !sign.right);
  build(tree, dir + "/right", depth - 1, on_path && sign.right);
}

} // namespace

Puzzle bst_puzzle_create(seed_t seed)
{
  const std::string tree_dir = "../resources/files-bst/tree";
  const int x = utils_rng_roll(ROOT_MIN, ROOT_MAX, seed);
  Tree tree{x, seed, "", 0};

  utils_remove_all(tree_dir); // Signs from an earlier run would otherwise be left in the tree.
  build(tree, tree_dir, DEPTH, true);

  std::string html_body = utils_html_printf("BST Puzzle", "../resources/files-bst/.desc.txt", {str(tree.x)});
  utils_generate_file("../resources/files-bst/instructions.html", html_body);
  return {"../resources/files-bst", html_body, str(tree.password), "Path: " + tree.path};
}
