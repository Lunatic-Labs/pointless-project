#include <iostream>
#include <algorithm>
#include <cmath>

#include "./include/puzzle.h"
#include "./include/utils.h"

/*** define puzzle sizes ***/
#define NUM_VALUES 11
#define VAL_SZ 4


/*** define indexes into values vector based on the base ***/
#define S8_START 0
#define S8_END 7

#define G16_START 8
#define G16_END 23

#define M36_START 24
#define M36_END 59

/*
* File: rematch-based-puzzle.cpp
* Author: Mekeal Brown
* Contact: brownmekeal@gmail.com
* Date: 2/6/2024
* Description:
*  The user is presented with an instructions page outlining the puzzle(cryptically).
*  There is an "Enter" button to press that takes them to the actual puzzle.
*  There are values scattered around the page and a vial. The user must fill the vial
*  by clicking the values in order from smallest to greatest.
*/



// indices  0-7 -> S8  8-23 -> G16  24-59 -> M38
static const std::vector<std::string> base_nums = {"!", "@", "#", "$", "%", "^", "&", "*", "{", "}", "[", "]", \
    "(", ")", "`", ">", "a", "b", "c", "d", "e", "f", "g", "h", "|", "~", ",", "/", "?", "_", "+", "=", "-", \
    ":", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};


//set indices into base_nums vector
void set_start_end(int base, int &start, int &end)
{
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
    default:
      std::cerr << "set_start_end failed..exiting\n";
      std::exit(0);
  }
}

//return the index of the symbol in relation to its base from base_nums
int find_val(int base, std::string symbol)
{
  int start, end;
  set_start_end(base, start, end);
  for(int i = 0; start <= end; start++) {
    if(base_nums[start] == symbol) {
      return i;
    }
    ++i;
  }
  std::cerr << "didnt find value...\n\n"; //shouldn't ever get here ofc
  std::exit(0);
  return -1;
}

//convert each value to base 10, puts it in "solved" vector
void values_solve(strvec_t vals, std::vector<int> bases, std::vector<int> &solved)
{
  for(int i = 0; i < NUM_VALUES; i++) {
    int sol = 0;
    std::vector<int> pos;
    for(int j = 0; j < 4; j++) {
      pos.push_back(find_val(bases[i], std::string(1, vals[i][j]))); //values index in its base's list of values
    }

    int pow = 3;
    for(int j = 0; j < 4; j++) {
      sol += pos[j] * std::pow(bases[i], pow--);
    }
    solved.push_back(sol);
  }
}

//creates a value, 4 numbers from a chosen base
void values_create(const std::vector<int> bases, strvec_t &values, long seed)
{
  for(auto it = bases.begin(); it != bases.end(); ++it) {
    auto& v = *it;
    int start, end;
    std::string cell_val;
    set_start_end(v, start, end);
    for(int i = 0; i < VAL_SZ; i++) {
      int num = utils_rng_roll(start, end, seed);
      cell_val += base_nums[num];
    }
    values.push_back(cell_val);
  }
}

Puzzle based_puzzle_create(long seed)
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
      default:
        std::cerr << "rematch_based_puzzle_create failed...exiting\n";
        std::exit(0);
    }
  }
  values_create(bases, values, seed);
  values_solve(values, bases, solved);

  //sort values from smallest to greatest
  std::vector<std::pair<int, std::string>> paired_vec;
  for(size_t i = 0; i < solved.size(); i++) {
    paired_vec.push_back(std::make_pair(solved[i], values[i]));
  }
  std::sort(paired_vec.begin(), paired_vec.end());
  for (const auto& pair : paired_vec) {
      values.push_back(pair.second);
      //std::cout << pair.second << "\n"; //for debug purposes
  }
  //std::cout << "\n";

  int key = utils_rng_roll(10000, 950000, seed);
  values.push_back(std::to_string(key));

  FLAGS |= (NO_HDR | NO_FTR);
  std::string html_content = utils_html_printf("Base Puzzle Rematch", "../html-txt/files-scrapped-based/.desc.txt", values);
  utils_generate_file("../html-txt/files-scrapped-based/instructions.html", html_content);

  FLAGS &= ~(NO_HDR | NO_FTR);
  return Puzzle{"files-scrapped-based", html_content, values.back(), {}};
}

/*
*  Symbolic Base-8 (S8):
*    This base uses eight ASCII symbols, each representing a digit from 0 to 7.
*    Symbols: !, @, #, $, %, ^, &, *
*    Example: #$! is 146 in decimal.
*
*  Glyphic Base-16 (G16):
*    This base utilizes sixteen ASCII symbols, representing digits from 0 to 9 and letters A to F.
*    Symbols: {, }, [, ], (, ), `, >, a, b, c, d, e, f, g, h
*    Example: [a is 170 in decimal.
*
*  Mystic Base-36 (M36):
*    Symbols: |, ~, ,, /, ?, *, +, =, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
*    Example:  |+= is 1684 in decimal.
*/
