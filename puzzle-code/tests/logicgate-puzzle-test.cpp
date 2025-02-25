#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool logicgate_puzzle_test()
{
  Puzzle test;

  std::cout << "starting logic gate puzzle tests" << std::endl;
  test = logicgate_puzzle_create(1);
  assert(test.password == "011001110111100");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = logicgate_puzzle_create(5);
  assert(test.password == "001101110011011");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = logicgate_puzzle_create(10);
  assert(test.password == "010101011001110");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = logicgate_puzzle_create(15);
  assert(test.password == "010000100001010");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "logic gate puzzle test successful\n" << std::endl;

  return true;
}