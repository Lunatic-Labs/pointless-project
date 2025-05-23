#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_maze_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  std::string important_content = "You vaguely recall that typing <code>instructions</code> into the <i><u><b>console</b></u></i> ";
  int seed = seed_gen("HelloHi@gmail.com");
  size_t found;

  test = rematch_maze_puzzle_create(1);
  assert(test.password == "1930886");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_maze_puzzle_create(5);
  assert(test.password == "1788765");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_maze_puzzle_create(10);
  assert(test.password == "7962008");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);
  
  test = rematch_maze_puzzle_create(15);
  assert(test.password == "6576550");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_maze_puzzle_create(seed);
  assert(test.password == "7114756");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  return true;
}