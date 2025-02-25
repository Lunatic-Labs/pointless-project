#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool pixel_puzzle_test()
{
  Puzzle test;

  std::cout << "starting pixel puzzle tests" << std::endl;
  test = pixel_puzzle_create(1);
  assert(test.password == "5400");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = pixel_puzzle_create(5);
  assert(test.password == "0");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = pixel_puzzle_create(10);
  assert(test.password == "360");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = pixel_puzzle_create(15);
  assert(test.password == "287496");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "pixel puzzle test successful\n" << std::endl;

  return true;
}