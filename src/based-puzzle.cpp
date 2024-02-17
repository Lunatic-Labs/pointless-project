#include <iostream>

#include "./include/puzzle.h"
#include "./include/utils.h"

////TODO: POTENTIAL PUZZLE REWORK //////////////////
///////////// STEPS TO SOLVE ///////////////////////
//convert each value from its base to base 2
//randomly choose between &, |, ^
//perform chosen operation on all values in the row
//count the number of bits that are on in the result
//number of on bits will be the solution for that row
//final solution is the values from the row solution

// Gotta beef it up. Not many solution permutations
/////////////////////// STEPS TO SOLVE ///////////////////////
//Convert values to base 10 
// Sum them up for each row 
// Convert the sum to base 2 
// Check if lsb is on or off 
// Save the resulting lsb values 
// Take that base 2 number and convert it back to base 10 
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

/*TODO: INCREASE PUZZLE SIZE(add 4 rows? want seven ?'s. add 1 column for the bitwise operator)
  -update .desc.txt(actual instructions and also the table of course)
*/
Puzzle based_puzzle_create(long seed)
{
  std::vector<std::string> values;
  std::vector<int> bases;

  for (int i = 0; i < 12; i++) {
    short rand = utils_rng_roll(0, 67, seed);
    if (rand > 42) { // Keep them within hex range of values
      if (rand > 47) {
        rand = utils_rng_roll(42, 47, seed);
      }

      values.push_back(std::string(1, static_cast<char>(rand + 23)));
      bases.push_back(16);
    } else {
      int base;

      if (rand >= 0 && rand <= 20) {
        base = 20;
      }
      else if (rand >= 21 && rand <= 42) {
        base = 31;
      }

      values.push_back(std::to_string(rand));
      bases.push_back(base);
    }
  }

  std::string html_content = utils_html_printf("Base Puzzle", "./files-based/.desc.txt", values);
  utils_generate_file("./files-based/instructions.html", html_content);

  return Puzzle{"files-based", std::to_string(solve(values, bases))};
}

