#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool maze_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  std::string token = "Lou's pixel art";
  int seed = seed_gen("HelloHi@gmail.com");
  size_t found;

  std::cout << "starting maze puzzle tests" << std::endl;
  test = maze_puzzle_create(1);
  assert(test.password == "u2r2dldr2u3rd2ruru3l2dluldl2urur5");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = maze_puzzle_create(5);
  assert(test.password == "r3u2ldl2urulu2r2d2r2druru2l2dlu2r3");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = maze_puzzle_create(10);
  assert(test.password == "rulu2r2dr2dldr2uru3l2dlululur2drurdru");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  
  test = maze_puzzle_create(15);
  assert(test.password == "r2ulurulu2lur2drdr2dl2d3ru2r2u3lur");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = maze_puzzle_create(seed);
  assert(test.password == "u2r2dldr2u3rd2ruru3l2dluldl2urur5");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  std::cout << "maze puzzle test successful\n" << std::endl;

  return true;
}