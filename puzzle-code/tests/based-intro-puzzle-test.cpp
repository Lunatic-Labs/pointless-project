#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool based_intro_puzzle_test()
{
  Puzzle test;

  std::cout << "starting based intro puzzle tests" << std::endl;
  test = based_intro_puzzle_create(1);
  assert(test.password == "5C32B727");
  std::cout << "test 1 completed\npassword = " << test.password<< std::endl;

  test = based_intro_puzzle_create(5);
  assert(test.password == "CD7A2F54");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = based_intro_puzzle_create(10);
  assert(test.password == "693F23E4");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = based_intro_puzzle_create(15);
  assert(test.password == "46FB9265");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "based intro puzzle test successful\n" << std::endl;

  return true;
}