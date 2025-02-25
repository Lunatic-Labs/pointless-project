#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool binary_addition_puzzle_test()
{
  Puzzle test;

  std::cout << "starting binary addition puzzle tests" << std::endl;
  test = binary_addition_puzzle_create(1);
  assert(test.password == "01110010");
  std::cout << "test 1 completed\npassword = " << test.password<< std::endl;

  test = binary_addition_puzzle_create(5);
  assert(test.password == "01011000");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = binary_addition_puzzle_create(10);
  assert(test.password == "01100011");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = binary_addition_puzzle_create(15);
  assert(test.password == "01001100");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "binary addition puzzle test successful\n" << std::endl;

  return true;
}