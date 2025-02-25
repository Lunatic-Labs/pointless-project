#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool math_puzzle_test()
{
  Puzzle test;

  std::cout << "starting math puzzle tests" << std::endl;
  test = math_puzzle_create(1);
  assert(test.password == "75");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = math_puzzle_create(5);
  assert(test.password == "82");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = math_puzzle_create(10);
  assert(test.password == "87");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = math_puzzle_create(15);
  assert(test.password == "97");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "math puzzle test successful\n" << std::endl;

  return true;
}