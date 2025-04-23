#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_puzzle_test()
{
  Puzzle test;
  int seed = seed_gen("HelloHi@gmail.com");

  test = rematch_puzzle_create(1);
  assert(test.password == "886538131");
  assert(test.contents_html == "");

  test = rematch_puzzle_create(5);
  assert(test.password == "765616451");
  assert(test.contents_html == "");

  test = rematch_puzzle_create(10);
  assert(test.password == "8607698");
  assert(test.contents_html == "");
  
  test = rematch_puzzle_create(15);
  assert(test.password == "550443168");
  assert(test.contents_html == "");

  test = rematch_puzzle_create(seed);
  assert(test.password == "756676914");
  assert(test.contents_html == "");

  return true;
}