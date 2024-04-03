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

std::string generate_sym_tbl(long seed)
{
  std::string from = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-_=+[]{};':<>,./?`~";
  std::string to   = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-_=+[]{};':<>,./?`~";

  std::shuffle(to.begin(), to.end(), std::default_random_engine(seed));

  std::string tbl= "";
  for(int i = 0; i < to.length(); i++) {
    tbl.append("  \"");
    tbl.append(1, from[i]);
    tbl.append("\": \"");
    tbl.append(1, to[i]);
    tbl.append("\",");
    tbl += "\n";
  }

  return tbl;
}

Puzzle rematch_encrypt_puzzle_create(long seed)
{
  std::string password = "changeme";
  std::string table = generate_sym_tbl(seed);
  // std::cout << table << std::endl;
  std::string html_body = utils_html_printf("Encrypt puzzle", "./files-rematch-encrypt/.desc.txt", {password, table});

  utils_generate_file("./files-rematch-encrypt/instructions.html", html_body);
  return {"files-rematch-encrypt", password, {}};
}
