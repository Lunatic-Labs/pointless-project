#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool color_puzzle_test()
{
  Puzzle test;

  std::cout << "starting color puzzle tests" << std::endl;
  test = color_puzzle_create(1);
  assert(test.password == "F4AA00");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = color_puzzle_create(5);
  assert(test.password == "331E54");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = color_puzzle_create(10);
  assert(test.password == "F4AA00");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = color_puzzle_create(15);
  assert(test.password == "F4AA00");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "color puzzle test successful\n" << std::endl;

  return true;
}