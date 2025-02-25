#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool maze_puzzle_test()
{
  Puzzle test;

  std::cout << "starting maze puzzle tests" << std::endl;
  test = maze_puzzle_create(1);
  assert(test.password == "u2r2dldr2u3rd2ruru3l2dluldl2urur5");
  std::cout << "test 1 completed\npassword = " << test.password<< std::endl;

  test = maze_puzzle_create(5);
  assert(test.password == "r3u2ldl2urulu2r2d2r2druru2l2dlu2r3");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = maze_puzzle_create(10);
  assert(test.password == "rulu2r2dr2dldr2uru3l2dlululur2drurdru");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = maze_puzzle_create(15);
  assert(test.password == "r2ulurulu2lur2drdr2dl2d3ru2r2u3lur");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "maze puzzle test successful\n" << std::endl;

  return true;
}