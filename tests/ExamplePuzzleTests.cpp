#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ExamplePuzzleTests
#include <boost/test/unit_test.hpp>
#include <stdio.h>
#include <fstream>
#include <string>
#include <zip.h>
#include "../puzzles/HelloWorld/HelloWorld.cpp"

BOOST_AUTO_TEST_CASE(testDoAllAttributesOfExamplePuzzleMakeSense)
{
    HelloWorld testPuzzle = HelloWorld();

    chdir("puzzles");
    std::string puzzleFile = testPuzzle.init("Example", "congrats.zip");
    BOOST_ASSERT(testPuzzle.getAnswer() == "4");
    std::unique_ptr<int> errPtr = nullptr;
    zip_t * testPuzzleZip = zip_open(puzzleFile.c_str(), 0, errPtr.get());
    BOOST_ASSERT(!errPtr.get()); // Assert no errors in opening zip
    BOOST_ASSERT(zip_fopen(testPuzzleZip, 
                           (testPuzzle.getName() + ".html").c_str(), 
                           0));
    zip_close(testPuzzleZip);
    BOOST_ASSERT(remove(puzzleFile.c_str()) == 0);
}