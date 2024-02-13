#include <iostream>

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

Puzzle ast_puzzle_create(long seed)
{
  (void)seed;

  std::string expr = "a+b*c+X/e";
  std::string path = evaluate_expr(expr, 6);

  std::cout << path << std::endl;

  utils_mkdir("./files-ast/expression");

  std::string html_body = utils_html_printf("AST Puzzle", "./files-ast/.desc.txt", {});

  return {};
}
