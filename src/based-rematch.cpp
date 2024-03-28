#include <iostream>
#include <algorithm>

#include "./include/puzzle.h"
#include "./include/utils.h"

//define puzzle sizes
#define GRID_CELLS 36 //6 x 6
#define CELL_SZ 4
//FIXME: why does changing the size to 4 break everything??


//define indexes into values vector based on the base
#define S8_START 0
#define S8_END 7

#define G16_START 8
#define G16_END 23

#define M36_START 24
#define M36_END 59



static const std::vector<std::string> values = {"!", "@", "#", "$", "%", "^", "&", "*", "{", "}", "[", "]", \
    "(", ")", "'\'", ">", "a", "b", "c", "d", "e", "f", "g", "h", "|", "~", ",", "/", "?", "_", "+", "=", "-", \
    ":", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};


//int solve(strvec_t vals, std::vector<int> base);


//creates value that will be placed in a cell in the table
void cells_create(const strvec_t values_str, const std::vector<int> bases, strvec_t *cells, long seed)
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
    for(int i = 0; i < CELL_SZ; i++) {
      int num = utils_rng_roll(start, end, seed);
      cell_val += values_str[num];
    }
    //std::cout << "cell val = " << cell_val << "\n";
    cells->push_back(cell_val);
  }
}


Puzzle based_rematch_puzzle_create(long seed)
{
  std::vector<int> bases; 
  strvec_t grid;
  for(int i = 0; i < GRID_CELLS; i++) {
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
  cells_create(values, bases, &grid, seed);

  std::string html_content = utils_html_printf("Base Puzzle Rematch", "./files-based-r/.desc.txt", grid);
  utils_generate_file("./files-based-r/instructions.html", html_content);

  return Puzzle{"files-based-r", "need to finish solve", {}};
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