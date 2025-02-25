#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_puzzle_test()
{
  Puzzle test;

  std::cout << "starting rematch puzzle tests" << std::endl;
  test = rematch_puzzle_create(1);
  assert(test.password == "886538131");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = rematch_puzzle_create(5);
  assert(test.password == "765616451");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = rematch_puzzle_create(10);
  assert(test.password == "8607698");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = rematch_puzzle_create(15);
  assert(test.password == "550443168");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "rematch puzzle test successful\n" << std::endl;

  return true;
}