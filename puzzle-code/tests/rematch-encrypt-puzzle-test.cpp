#include <iostream>
#include <string>
#include <cassert>
#include "./include/test.h"
#include "./include/file.h"
#include "../src/include/puzzle.h"
#include "../src/include/utils.h"

bool rematch_encrypt_puzzle_test()
{
  Puzzle test;
  std::string header_content = file_contents("../html-txt/resources/header.txt");
  std::string footer_content = file_contents("../html-txt/resources/footer.txt");
  std::string important_content = "All we have are the magic machines that encrypted the password.";
  int seed = seed_gen("HelloHi@gmail.com");
  size_t found;

  std::cout << "starting rematch encrypt puzzle tests" << std::endl;
  test = rematch_encrypt_puzzle_create(1);
  assert(test.password == "procedure");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_encrypt_puzzle_create(5);
  assert(test.password == "algorithm");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_encrypt_puzzle_create(10);
  assert(test.password == "program");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);
  
  test = rematch_encrypt_puzzle_create(15);
  assert(test.password == "function");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);

  test = rematch_encrypt_puzzle_create(seed);
  assert(test.password == "theory");
  found = test.contents_html.find(header_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(footer_content);
  assert(found != std::string::npos);
  found = test.contents_html.find(important_content);
  assert(found != std::string::npos);
  std::cout << "rematch encrypt puzzle test successful\n" << std::endl;

  return true;
}