#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool binary_addition_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  std::string token = "master key";
  std::string important_content = "<li><span class=\"state_red\"></span>E:↓A</li>";
  int seed = seed_gen("HelloHi@gmail.com");
  size_t found;

  test = binary_addition_puzzle_create(1);
  assert(test.password == "01110010");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = binary_addition_puzzle_create(5);
  assert(test.password == "01011000");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = binary_addition_puzzle_create(10);
  assert(test.password == "01100011");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);
  
  test = binary_addition_puzzle_create(15);
  assert(test.password == "01001100");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = binary_addition_puzzle_create(seed);
  assert(test.password == "00110110");
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