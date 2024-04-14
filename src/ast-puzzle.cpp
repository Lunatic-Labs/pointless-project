/*
 * File: ast-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Contact: zdhdev@yahoo.com, tcaustin@mail.lipscomb.edu
 * Date: 1/19/2024
 * Description:
 *   The user is presented with a generated expression as well as the destination variable.
 *   They must create and navigate the abstract syntax tree to find the correct path from the root to the destination.
 */

#include <iostream>
#include <algorithm>
#include <cassert>

#include "./include/puzzle.h"
#include "./include/utils.h"

// (1) exp -> exp + term
// (2) exp -> exp – term
// (3) exp -> term
// (4) term -> term * fact
// (5) term -> term / fact
// (6) term -> fact
// (7) fact -> ID
// (8) fact -> ( exp )

enum class State {
  Expression,
  Term,
  Factor,
  ID
};

static void balance_parens(char *&it, int &balance, State &state, std::string &path)
{
  while (*it) {
    if (*it == '(') {
      balance++;
    }
    else if (*it == ')' && balance == 0) {
      assert((state != State::ID) && (state != State::Factor));
      if (state == State::Term) {
        path += "T";
      }
      path += "EF";
      state = State::Term;
      break;
    }
    else if (*it == ')') {
      balance--;
    }
    else if (balance == 0) {
      break;
    }
    it++;
  }
}

static std::string evaluate_expr(std::string str, size_t index)
{
  std::string path = "";

  int balance = 0;

  char *it = &str[index];
  State state = State::ID;

  while (*it) {
    switch (state) {
      case State::ID: {
        path += "I";
        state = State::Factor;
        break;
      }
      case State::Factor: {
        path += "F";
        state = State::Term;
        break;
      }
      case State::Term:
        balance_parens(it, balance, state, path);
        if (*it == '*' || *it == '/') {
          path += "T";
        }
        if (*it == '-' || *it == '+') {
          path += "TE";
          state = State::Expression;
        }
        it++;
        break;
      case State::Expression: {
        balance_parens(it, balance, state, path);
        if (*it == '-' || *it == '+') {
          path += "E";
        }
        it++;
        break;
      }
      default:
        assert(false && "evaluate_expr: unreachable");
    }
  }
  if(state == State::Term) {
    path += "TE";
  }
  if(state == State::Expression) {
    path += "E";
  }

  return path;
}

static int fail_safe = 0;
static int password;
static long mutable_seed;

static void create_dirs(std::string path, int depth, const char *ans, State state)
{
  if (fail_safe++ > 9000) {
    std::cout << "HIT FAIL SAFE\n";
    return;
  }
  if(depth < -10) {
    std::cout << "Error: depth < -10" << std::endl;
    return;
  }
  utils_mkdir(path);

  switch (state) {
    case State::Expression: {
      create_dirs(path + "/Term", depth - 1, *ans == 'T' ? ans+1 : "x", State::Term);
      if (depth <= 0) return;
      create_dirs(path + "/Expression", depth - 1, *ans == 'E' ? ans+1 : "x", State::Expression);
      break;
    }
    case State::Term: {
      create_dirs(path + "/Factor", depth - 1, *ans == 'F' ? ans+1 : "x", State::Factor);
      if (depth <= 0) return;
      create_dirs(path + "/Term", depth - 1, *ans == 'T' ? ans+1 : "x", State::Term);
      break;
    }
    case State::Factor: {
      create_dirs(path + "/ID", depth - 1, *ans == 'I' ? ans+1 : "x", State::ID);
      if (depth <= 0) return;
      create_dirs(path + "/(Expression)", depth - 1, *ans == 'E' ? ans+1 : "x", State::Expression);
      break;
    }
    case State::ID: {
      int rng = utils_rng_roll(1000000,9000000, mutable_seed);
      utils_generate_file(path + "/ID.txt", std::to_string(rng));
      if(!*ans) { // We have consumed the answer string, so this is the correct file.
        password = rng;
      }
      break;
    }
    default:
      std::cout << "Error: UNREACHABLE" << std::endl;
      break;
  }
}

static std::string generate_expr(long seed)
{
  std::string expr;
  std::string ops = "+-/*";
  std::vector<std::string> expr_forms = {
    "I+(I+I)+I+((I+I)+I)",
    "(I+(I+I))+I+(I+I+I)",
    "(I+(I+I))+I+(I+I)+I",
    "((I+I+I))+I+(I+I)+I",
    "((I+I+I))+(I+I+I)+I",
    "I+(I+I)+(I+I+I)+I+I",
  };

  expr = expr_forms[utils_rng_roll(0, expr_forms.size()-1, seed)];

  char c = 'A';
  for (size_t i = 0; i < expr.size(); ++i) {
    int rng = utils_rng_roll(0, ops.size()-1, seed);
    if (expr[i] == 'I') {
      expr[i] = c;
      ++c;
    }
    else if (expr[i] == '+') {
      expr[i] = ops[rng];
    }
  }

  return expr;
}

static size_t find_best_guy(std::string &expr, size_t max_len)
{
  size_t best = 0;
  size_t best_len = 0;
  for (size_t i = 0; i < expr.size(); ++i) {
    if (!isalpha(expr[i])) {
      continue;
    }
    std::string path = evaluate_expr(expr, i);
    if (path.size() > best_len && path.size() <= max_len) {
      best = i;
      best_len = path.size();
    }
  }
  return best;
}

Puzzle ast_puzzle_create(long seed)
{
  mutable_seed = seed;
  const size_t depth = 10;
  const size_t difficulty = 10;

  std::string expr = generate_expr(seed);
  size_t best = find_best_guy(expr, difficulty);
  std::string guy;
  guy += expr[best];

  std::string correct_path = evaluate_expr(expr, best);
  std::reverse(correct_path.begin(), correct_path.end());

  utils_mkdir("./files-ast/AST");
  create_dirs("./files-ast/AST", depth, correct_path.c_str(), State::Expression);

  std::string html_body = utils_html_printf("AST Puzzle", "./files-ast/.desc.txt", { expr, guy });
  utils_generate_file("./files-ast/instructions.html", html_body);

  return {"files-ast", std::to_string(password), " Path: " + correct_path};
}

//(A+B)*C
