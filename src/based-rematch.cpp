#include <iostream>
#include <algorithm>

#include "./include/puzzle.h"
#include "./include/utils.h"



#define GRID_CELLS 36 //6 x 6

static const std::vector<std::string> S8 = {"!", "@", "#", "$", "%", "^", "&", "*"};
static const std::vector<std::string> G16 = {"{", "}", "[", "]", "(", ")", "<", ">", "a", "b", "c", "d", "e", "f", "g", "h"};
static const std::vector<std::string> M36 = {"|", "~", ",", "/", "?", "_", "+", "=", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};


int solve(std::vector<std::string> &vals, std::vector<int> &base);


//creates value that will be placed in a cell in the table
void cell_create(const std::vector<std::string> v, std::vector<std::string, int> *grid, int base);


Puzzle based_rematch_puzzle_create(long seed)
{
  std::vector<std::string, int> grid; //hold the value/its base as a pair
  for(int i = 0; i < GRID_CELLS; i++) {
    int num = utils_rng_roll(0, 2, seed);
    switch (num)
    {
    case 0:
      cell_create(S8, &grid, 8);
      break;
    case 1:
      cell_create(G16, &grid, 16);
      break;
    case 2:
      cell_create(M36, &grid, 36);
      break;
    }
    //randomly choose one of the bases(utils_rng_roll(seed))
    //randomly choose 3 symbols from the base and concatenate them into a string(pass to cell create to keep code DRY)
    //push those strings into a vector for use in the solver(iterate through vector solving each string)
    //push the base into a vector for use in the solver
  }

  return Puzzle{"files-based-r", "some stuff here ofc", {}};
}


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