#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool logicgate_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  size_t found;

  std::cout << "starting logic gate puzzle tests" << std::endl;
  test = logicgate_puzzle_create(1);
  assert(test.password == "011001110111100");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);

  test = logicgate_puzzle_create(5);
  assert(test.password == "001101110011011");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);

  test = logicgate_puzzle_create(10);
  assert(test.password == "010101011001110");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  
  test = logicgate_puzzle_create(15);
  assert(test.password == "010000100001010");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  std::cout << "logic gate puzzle test successful\n" << std::endl;

  return true;
}