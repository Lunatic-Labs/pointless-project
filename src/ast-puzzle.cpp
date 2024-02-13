#include <iostream>
#include <algorithm>
#include <fstream>

#include "./include/puzzle.h"
#include "./include/utils.h"

// (ID + ( (ID * ID) * ID ) - ID)
//  a+b*c/d+e

// (1) exp → exp + term
// (2) exp → exp – term
// (3) exp → term
// (4) term → term * fact
// (5) term → term / fact
// (6) term → fact
// (7) fact → ID
// (8) fact → ( exp )

std::string evaluate_expr(std::string str, int index)
{
  std::string path = "IFT";

  char *it = &str[index+1];

  while (*it && (*it == '*' || *it == '/')) {
    path += "T";
    it += 2;
  }

  path += "E";

  while (*it) {
    if(*it == '+' || *it == '-')
      path += "E";
    it += 2;
  }

  return path;
}

enum class State {
  Expression,
  Term,
  Factor,
  ID
};

void create_dirs(std::string path, int depth, const char *ans, State state)
{
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
      break;
    }
    case State::ID: {
      if(!*ans) {
        utils_generate_file(path + "/ID.txt", "winner");
        std::cout << "winner exists here: " << path << std::endl;
      }
      else
        utils_generate_file(path + "/ID.txt", "trash");
      break;
    }
    default:
      std::cout << "Error: UNREACHABLE" << std::endl;
      break;
  }
}

Puzzle ast_puzzle_create(long seed)
{
  (void)seed;

  std::string expr = "a+b*c+X/e+t/6/1/0+2-4";
  std::string ans = evaluate_expr(expr, 6);
  std::reverse(ans.begin(), ans.end());

  std::cout << "ans: " << ans << std::endl;

  utils_mkdir("./files-ast/AST");
  create_dirs("./files-ast/AST", 50, ans.c_str(), State::Expression);

  std::string html_body = utils_html_printf("AST Puzzle", "./files-ast/.desc.txt", { expr });
  utils_generate_file("./files-ast/instructions.html", html_body);

  return {"files-ast", ans};
}
