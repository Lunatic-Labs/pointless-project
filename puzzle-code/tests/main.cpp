#include <iostream>
#include <vector>
#include <string>
#include "./include/test.h"
#include "../src/include/utils.h"

uint32_t FLAGS = 0;

int main()
{
  FLAGS |= ANS_ONLY;

  std::vector<bool> tests = {
    math_puzzle_test(),
    color_puzzle_test(),
    pixel_puzzle_test(),
    maze_puzzle_test(),
    based_intro_puzzle_test(),
    encrypt_puzzle_test(),
    rematch_puzzle_test(),
    rematch_maze_puzzle_test(),
    rematch_encrypt_puzzle_test(),
    rematch_based_puzzle_test(),
    binary_addition_puzzle_test(),
    logicgate_puzzle_test(),
    bst_puzzle_test(),
    fin_puzzle_test(),
  };
  std::cout << "automated testing completed." << std::endl;

  return 0;
}