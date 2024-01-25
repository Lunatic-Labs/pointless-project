#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HtmlGeneratorTests
#include <boost/test/unit_test.hpp>
#include <stdio.h>
#include <fstream>
#include "../include/html-generator.h"

BOOST_AUTO_TEST_CASE(test_can_generate_be_called)
{
  chdir("puzzles");
  try {
    BOOST_ASSERT(generate_html("title", "test", 0) == "title.html");
    BOOST_ASSERT(remove("title.html") == 0);
  }
  catch(std::exception e) {
    std::cerr << e.what();
  }
}

BOOST_AUTO_TEST_CASE(test_does_generate_populate_html_file)
{
  try {
    std::string file_path = generate_html("test", "abc", 0);
    std::ifstream file = std::ifstream(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    BOOST_ASSERT(buffer.str().length() >= 7);
    BOOST_ASSERT(remove(file_path.c_str()) == 0);
  }
  catch (std::exception e) {
    std::cerr << e.what();
  }
}

BOOST_AUTO_TEST_CASE(test_does_generate_put_title_in_html_file)
{
  try {
    std::string file_path = generate_html("test", "abc", 0);
    std::ifstream file = std::ifstream(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    BOOST_ASSERT(buffer.str().find("test") != std::string::npos);
    BOOST_ASSERT(remove(file_path.c_str()) == 0);
  }
  catch (std::exception e) {
    std::cerr << e.what();
  }

}

BOOST_AUTO_TEST_CASE(test_does_generate_put_description_in_html_file)
{
  try {
    std::string file_path = generate_html("test", "abc", 0);
    std::ifstream file = std::ifstream(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    BOOST_ASSERT(buffer.str().find("abc") != std::string::npos);
    BOOST_ASSERT(remove(file_path.c_str()) == 0);
  }
  catch (std::exception e) {
    std::cerr << e.what();
  }
}

BOOST_AUTO_TEST_CASE(test_does_generate_put_seed_in_html_file)
{
  try {
    std::string file_path = generate_html("test", "abc", 6578293);
    std::ifstream file = std::ifstream(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    BOOST_ASSERT(buffer.str().find("6578293") != std::string::npos);
    BOOST_ASSERT(remove(file_path.c_str()) == 0);
  }
  catch (std::exception e) {
    std::cerr << e.what();
  }
}

