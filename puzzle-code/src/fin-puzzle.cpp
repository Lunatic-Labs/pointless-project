#include "./include/puzzle.h"
#include "./include/utils.h"

Puzzle fin_puzzle_create(seed_t seed)
{
  const std::string token = utils_token(utils_derive_seed(seed, "token"));

  std::string html_content = utils_html_printf("Goodbye Pointless", "../resources/files-fin/.desc.txt", {}, token);
  utils_generate_file("../resources/files-fin/instructions.html", html_content);
  return {"../resources/files-fin", html_content, "", token, {}};
}
