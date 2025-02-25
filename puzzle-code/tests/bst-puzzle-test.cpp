#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool bst_puzzle_test()
{
  Puzzle test;

  std::cout << "starting BST puzzle tests" << std::endl;
  test = bst_puzzle_create(1);
  assert(test.password == "4299662");
  assert(test.extra_info == " Path: lrlrlrrrll");
  std::cout << "test 1 completed\npassword = " << test.password << ", extra info =" << test.extra_info->c_str() << std::endl;

  test = bst_puzzle_create(5);
  assert(test.password == "1403321");
  assert(test.extra_info == " Path: rrrllrrlrl");
  std::cout << "test 2 completed\npassword = " << test.password << ", extra info =" << test.extra_info->c_str() << std::endl;

  test = bst_puzzle_create(10);
  assert(test.password == "6118173");
  assert(test.extra_info == " Path: rrlrrlrrlr");
  std::cout << "test 3 completed\npassword = " << test.password << ", extra info =" << test.extra_info->c_str() << std::endl;
  
  test = bst_puzzle_create(15);
  assert(test.password == "2538609");
  assert(test.extra_info == " Path: llrllrlllr");
  std::cout << "test 4 completed\npassword = " << test.password << ", extra info =" << test.extra_info->c_str() << std::endl;
  std::cout << "BST puzzle test successful\n" << std::endl;

  return true;
}