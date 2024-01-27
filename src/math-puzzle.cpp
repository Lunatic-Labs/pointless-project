#include "./include/math-puzzle.h"
#include "./include/utils.h"

Puzzle math_puzzle_create(long seed)
{
  // TODO: Change in final version to bigger numbers.
  int a = utils_rng_roll(1, 5, seed);
  int b = utils_rng_roll(6, 9, seed);
  int s = a+b;
  std::string description = utils_file_to_str("./files-math/.desc.txt");
  std::string question = BOLD("what is " + std::to_string(a) + "+" + std::to_string(b) + "?");
  description.append(question);
  
  std::string html_content = utils_generate_html("Hello Pointless", description, seed);

  utils_generate_file("./files-math/instructions.html", html_content);
  return Puzzle{"files-math", std::to_string(s)};
}