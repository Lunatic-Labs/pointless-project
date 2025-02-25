#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool encrypt_puzzle_test()
{
  Puzzle test;

  std::cout << "starting encrypt puzzle tests" << std::endl;
  test = encrypt_puzzle_create(1);
  assert(test.password == "greedy_algorithms");
  std::cout << "test 1 completed\npassword = " << test.password<< std::endl;

  test = encrypt_puzzle_create(5);
  assert(test.password == "recursive_function");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = encrypt_puzzle_create(10);
  assert(test.password == "breadth_first_search");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = encrypt_puzzle_create(15);
  assert(test.password == "programming_languages");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "encrypt puzzle test successful\n" << std::endl;

  return true;
}