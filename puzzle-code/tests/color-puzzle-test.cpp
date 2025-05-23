#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool color_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  std::string token = "brute force";
  std::string important_content = "When defining a color in hexadecimal, red is the first two numbers, ";
  int seed = seed_gen("HelloHi@gmail.com");
  size_t found;

  test = color_puzzle_create(1);
  assert(test.password == "F4AA00");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = color_puzzle_create(5);
  assert(test.password == "331E54");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = color_puzzle_create(10);
  assert(test.password == "F4AA00");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);
  
  test = color_puzzle_create(15);
  assert(test.password == "F4AA00");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = color_puzzle_create(seed);
  assert(test.password == "F4AA00");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  return true;
}