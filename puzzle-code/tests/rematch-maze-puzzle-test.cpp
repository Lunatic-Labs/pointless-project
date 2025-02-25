#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_maze_puzzle_test()
{
  Puzzle test;

  std::cout << "starting rematch maze puzzle tests" << std::endl;
  test = rematch_maze_puzzle_create(1);
  assert(test.password == "1930886");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = rematch_maze_puzzle_create(5);
  assert(test.password == "1788765");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = rematch_maze_puzzle_create(10);
  assert(test.password == "7962008");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = rematch_maze_puzzle_create(15);
  assert(test.password == "6576550");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "rematch maze puzzle test successful\n" << std::endl;

  return true;
}