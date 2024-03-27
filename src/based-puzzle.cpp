#include <iostream>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define CELLS 12
#define RAND_MAX 67 //0-42, A-Z(0, 67)


/*
* * * * STEPS TO SOLVE * * * *
* Convert values to base 10 
* Sum them up for each row 
* Convert the sum to base 2 
* Check if lsb is on or off 
* Save the resulting lsb values 
* Take that base 2 number and convert it back to base 10 
*/
int solve(std::vector<std::string> &vals, std::vector<int> &base)
{
  int decimalval, items, solution, mask;
  decimalval = items = solution = 0;
  mask = 8;

  std::vector<std::string>::iterator it;
  std::vector<int>::iterator it2;
  for (it = vals.begin(), it2 = base.begin(); it != vals.end() && it2 != base.end(); ++it, ++it2) {
    decimalval += std::stoi(*it, nullptr, *it2);
    ++items;
    if (items == 3) {
      if (decimalval & 1) {
        solution |= mask;
      }
      mask >>= 1;
      decimalval = items = 0;
    }
  }
  return solution;
}

Puzzle based_puzzle_create(long seed)
{
  std::vector<std::string> values;
  std::vector<int> bases;

  for (int i = 0; i < CELLS; i++) {
    short rand = utils_rng_roll(0, RAND_MAX, seed);
    if (rand > 42) { // Keep them within hex range of values, 42-47 == A-F
      if (rand > 47) {
        rand = utils_rng_roll(42, 47, seed);
      }

      values.push_back(std::string(1, static_cast<char>(rand + 23))); // + 23 to get correct ASCII value
      bases.push_back(16);
    } else {
      int base;

      if (rand >= 0 && rand <= 20) { //0-20 is base 20
        base = 20;
      }
      else if (rand >= 21 && rand <= 42) { //21-42 is base 31
        base = 31;
      }

      values.push_back(std::to_string(rand));
      bases.push_back(base);
    }
  }

  std::string html_content = utils_html_printf("Base Puzzle", "./files-based/.desc.txt", values);
  utils_generate_file("./files-based/instructions.html", html_content);

  return Puzzle{"files-based", std::to_string(solve(values, bases)), {}};
}

