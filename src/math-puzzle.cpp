#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle math_puzzle_create(long seed)
{
  int a = utils_rng_roll(1, 6, seed);
  int b = utils_rng_roll(10, 19, seed);
  int s = a+b;
  Puzzle p{"files-math", std::to_string(s)};

  if (FLAGS & ANS_ONLY) {
    return p;
  }

  std::string html_content = utils_html_printf("Hello Pointless", "./files-math/.desc.txt",
                                                {std::to_string(a), std::to_string(b)});
  utils_generate_file("./files-math/instructions.html", html_content);
  return p;
}