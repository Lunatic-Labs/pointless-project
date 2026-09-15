#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool fin_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../resources/templates/header.txt");
  std::string fin_content = file_contents("../resources/files-fin/.desc.txt");
  std::string footer_content = file_contents("../resources/templates/footer.txt");
  std::string token = "pointless journey";
  long seed = utils_seed_from_email("HelloHi@gmail.com");
  size_t found;

  test = fin_puzzle_create(1);
  assert(test.password == "");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(fin_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = fin_puzzle_create(5);
  assert(test.password == "");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(fin_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = fin_puzzle_create(10);
  assert(test.password == "");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(fin_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);
  
  test = fin_puzzle_create(15);
  assert(test.password == "");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(fin_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  test = fin_puzzle_create(seed);
  assert(test.password == "");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(fin_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(token);
  assert(found != std::string::npos);

  return true;
}