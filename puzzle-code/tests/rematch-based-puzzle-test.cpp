#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_based_puzzle_test()
{
  Puzzle test;

  std::cout << "starting rematch based puzzle tests" << std::endl;
  test = rematch_based_puzzle_create(1);
  assert(test.password == "196");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = rematch_based_puzzle_create(5);
  assert(test.password == "28");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = rematch_based_puzzle_create(10);
  assert(test.password == "118");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = rematch_based_puzzle_create(15);
  assert(test.password == "169");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "rematch based puzzle test successful\n" << std::endl;

  return true;
}