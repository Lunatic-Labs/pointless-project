#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ExamplePuzzleTests
#include <boost/test/unit_test.hpp>
#include <stdio.h>
#include <fstream>
#include <string>
#include <zip.h>
#include "../puzzles/hello-world/hello-world.cpp"

BOOST_AUTO_TEST_CASE(test_do_all_attributes_of_example_puzzle_make_sense)
{
    HelloWorld test_puzzle = HelloWorld();

    chdir("puzzles");
    std::string puzzle_file = test_puzzle.init("Example", "congrats.zip");
    BOOST_ASSERT(test_puzzle.get_answer() == "4");
    std::unique_ptr<int> err_ptr = nullptr;
    zip_t * test_puzzle_zip = zip_open(puzzle_file.c_str(), 0, err_ptr.get());
    BOOST_ASSERT(!err_ptr.get()); // Assert no errors in opening zip
    BOOST_ASSERT(zip_fopen(test_puzzle_zip, (test_puzzle.get_name() + ".html").c_str(), 0));
    zip_close(test_puzzle_zip);
    BOOST_ASSERT(remove(puzzle_file.c_str()) == 0);
}

