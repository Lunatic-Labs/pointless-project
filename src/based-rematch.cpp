#include <iostream>
#include <algorithm>
#include <cmath>

#include "./include/puzzle.h"
#include "./include/utils.h"

//define puzzle sizes
#define NUM_VALUES 11
#define VAL_SZ 4


//define indexes into values vector based on the base
#define S8_START 0
#define S8_END 7

#define G16_START 8
#define G16_END 23

#define M36_START 24
#define M36_END 59



static const std::vector<std::string> base_nums = {"!", "@", "#", "$", "%", "^", "&", "*", "{", "}", "[", "]", \
    "(", ")", "`", ">", "a", "b", "c", "d", "e", "f", "g", "h", "|", "~", ",", "/", "?", "_", "+", "=", "-", \
    ":", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};



/*
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
*/

//void sort(strvec_t )


//return the index of the symbol in relation to its base
int find_val(int base, std::string symbol)
{
  int start, end;
  switch(base)
  {
    case 8:
      start = S8_START;
      end = S8_END;
      break;
    case 16:
      start = G16_START;
      end = G16_END;
      break;
    case 36:
      start = M36_START;
      end = M36_END;
      break;
  }
  int i = 0;
  for(start; start <= end; start++) {
    if(base_nums[start] == symbol) {
      return i;
    }
    ++i;
  }
  std::cout << "didnt find value...\n\n";
  return -1;
}

void values_solve(strvec_t vals, std::vector<int> bases, std::vector<int> *solved)
{
  for(int i = 0; i < NUM_VALUES; i++) {
    int sol = 0;
    std::vector<int> pos;
    for(int j = 0; j < 4; j++) {
      pos.push_back(find_val(bases[i], std::string(1, vals[i][j]))); //values index in its list of values
    }

    int pow = 3;
    for(int j = 0; j < 4; j++) {
      sol += pos[j] * std::pow(bases[i], pow--);
    }
    solved->push_back(sol);
  }
}

//creates a value, 4 numbers from a chosen base
void values_create(const std::vector<int> bases, strvec_t *values, long seed)
{
  for(auto it = bases.begin(); it != bases.end(); ++it) {
    auto& v = *it;
    int start, end;
    std::string cell_val;
    switch(v)
    {
      case 8:
        start = S8_START;
        end = S8_END;
        break;
      case 16:
        start = G16_START;
        end = G16_END;
        break;
      case 36:
        start = M36_START;
        end = M36_END;
        break;
    }
    for(int i = 0; i < VAL_SZ; i++) {
      int num = utils_rng_roll(start, end, seed);
      cell_val += base_nums[num];
    }
    values->push_back(cell_val);
  }
}


Puzzle based_rematch_puzzle_create(long seed)
{
  std::vector<int> bases, solved; 
  strvec_t values;
  for(int i = 0; i < NUM_VALUES; i++) {
    int num = utils_rng_roll(0, 2, seed);
    switch (num) //choose base based...nice...on random value
    {
      case 0:
        bases.push_back(8);
        break;
      case 1:
        bases.push_back(16);
        break;
      case 2:
        bases.push_back(36);
        break;
    }
  }
  values_create(bases, &values, seed);
  values_solve(values, bases, &solved);

  int iter = 0;
  for(std::string s: values) {
    std::cout << s << "\n";
  }
  std::cout << "\n";

  //TODO: VERIFY THAT THIS SORTING IS ACCURATE
  //sort values from smallest to greatest
  std::vector<std::pair<int, std::string>> paired_vec;
  for(size_t i = 0; i < solved.size(); i++) {
    paired_vec.push_back(std::make_pair(solved[i], values[i]));
  }
  std::sort(paired_vec.begin(), paired_vec.end());
  for (const auto& pair : paired_vec) {
      values.push_back(pair.second);
  }

  int key = utils_rng_roll(10000, 150000, seed);
  values.push_back(std::to_string(key));

  //debug purposes ofc
  iter = 0;
  for(std::string s: values) {
    if(iter++ <= 10)
      continue;
    std::cout << s << "\n";
  }
  std::cout << "\n";



  std::string html_content = utils_html_printf("Base Puzzle Rematch", "./files-based-r/.desc.txt", values);
  utils_generate_file("./files-based-r/instructions.html", html_content);

  return Puzzle{"files-based-r", values.back(), {}};
}

/*TODO: maybe have the puzzle not require all of the bases to be converted. 
* maybe have each row have a certain path you can take through the puzzle to "turn on" the spot with the question mark
* the decoding of the cell you land on tells you which direction to go somehow
*/


/*
*  Symbolic Base-8 (S8):
*    This base uses eight ASCII symbols, each representing a digit from 0 to 7.
*    Symbols: !, @, #, $, %, ^, &, *
*    Example: #$! is 146 in decimal.
*
*  Glyphic Base-16 (G16):
*    This base utilizes sixteen ASCII symbols, representing digits from 0 to 9 and letters A to F.
*    Symbols: {, }, [, ], (, ), <, >, a, b, c, d, e, f, g, h
*    Example: [a is 170 in decimal.
*
*  Mystic Base-36 (M36):
*    Symbols: |, ~, , /, ?, *, +, =, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
*    Example:  |+= is 1684 in decimal.
*/




/*TODO: Make table bigger(6x6 would be good)
*        3 symbols of same base per table box
*        hidden elements on page that are only visible when the user hovers over it(little hints maybe?)
*        clicking on the highlighted element will cause the hint text to appear(probably just the different bases' symbols enumerated)
*/



/*
* You have found yourself in a dark dungeon. There is a mysterious glowing obelisk in the center of the room.
* In order to leave this dungeon, the obelisk must be filled, but not in any old fashion.
* Your challenge is quite base-ic, really. Collect the pieces strewn about the room to fill the obelisk. 
*/