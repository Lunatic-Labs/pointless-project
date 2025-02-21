#include <iostream>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define CELLS 24
#define MAX_RAND 67 //0-42, A-Z(0, 67)

/*
* File: rematch-based-puzzle.cpp
* Author: Mekeal Brown
* Contact: brownmekeal@gmail.com
* Date: 2/6/2024
* Description:
*  The user is presented told the mappings of three unique bases.
*  There is a table with values presented in one of the three bases.
*  They must perform the following steps to solve:
*   -Convert values to base 10
*   - Sum them up for each row
*   -Convert the sum to base 2
*   -Check if lsb is on or off
*   -Save the resulting lsb values
*   -Take that base 2 number and convert it back to base 10
*   
*   CHANGES BY JORDAN
*   This puzzle is a bit too easy to brute force. 
*/


static int solve(std::vector<std::string> &vals, std::vector<int> &base)
{
  int decimalval, items, solution, mask;
  decimalval = items = solution = 0;
  mask = 128;

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

Puzzle rematch_based_puzzle_create(long seed)
{
  std::vector<std::string> values;
  std::vector<int> bases;

  for (int i = 0; i < CELLS; i++) {
    short rand = utils_rng_roll(0, MAX_RAND, seed);
    if (rand > 42) { // Keep them within hex range of values* Convert values to base 10
      if (rand > 47) {
        rand = utils_rng_roll(42, 47, seed);
      }

      values.push_back(std::string(1, static_cast<char>(rand + 23))); // + 23 to get correct ASCII value
      bases.push_back(16);
    } else {
      int base = 0;

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

  std::string html_content = utils_html_printf("Base Puzzle Rematch", "./files-rematch-based/.desc.txt", values);
  utils_generate_file("./files-rematch-based/instructions.html", html_content);

  return Puzzle{"files-rematch-based", std::to_string(solve(values, bases)), {}};
}
