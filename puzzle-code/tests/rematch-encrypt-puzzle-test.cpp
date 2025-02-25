#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_encrypt_puzzle_test()
{
  Puzzle test;

  std::cout << "starting rematch encrypt puzzle tests" << std::endl;
  test = rematch_encrypt_puzzle_create(1);
  assert(test.password == "procedure");
  std::cout << "test 1 completed\npassword = " << test.password << std::endl;

  test = rematch_encrypt_puzzle_create(5);
  assert(test.password == "algorithm");
  std::cout << "test 2 completed\npassword = " << test.password << std::endl;

  test = rematch_encrypt_puzzle_create(10);
  assert(test.password == "program");
  std::cout << "test 3 completed\npassword = " << test.password << std::endl;
  
  test = rematch_encrypt_puzzle_create(15);
  assert(test.password == "function");
  std::cout << "test 4 completed\npassword = " << test.password << std::endl;
  std::cout << "rematch encrypt puzzle test successful\n" << std::endl;

  return true;
}