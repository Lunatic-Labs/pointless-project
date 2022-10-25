#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PuzzleTests
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "../util/Puzzle.h"
#include <vector>
#include <string>
#include <stdio.h>
#include "../util/zip/Zipper.h"

class PuzzleFixture : public Puzzle 
{
protected:
    void setAnswer() 
    {
        answer = "abc123";
    }
    std::vector<std::string> generateFiles(std::string containedZipPath)
    {
        std::ofstream out("hello.txt");
        out << "Hi! The seed to this puzzle is " << getSeed();
        out.close();
        return {containedZipPath, "hello.txt"};
    }
};

BOOST_AUTO_TEST_CASE(setup) 
{
    std::ofstream out("congrats.txt");
    out << "Woo! You completed the puzzle!";
    out.close();
    
    zipFiles("congrats.zip", {"congrats.txt"});
}

BOOST_AUTO_TEST_CASE(testCanPuzzleBeInitialized) 
{
    PuzzleFixture a = PuzzleFixture();
    BOOST_ASSERT(a.init("test", "congrats.zip", 0) == "test.zip");
}

BOOST_AUTO_TEST_CASE(testCanPuzzleBeInitializedWithPreviousZipFile)
{
    PuzzleFixture a = PuzzleFixture();
    BOOST_ASSERT(a.init("test2", "test.zip", 0) == "test2.zip");
}

BOOST_AUTO_TEST_CASE(testCanPuzzleBeInitializedWithNoSeed) 
{
    PuzzleFixture a = PuzzleFixture();
    BOOST_ASSERT(a.init("test3", "test2.zip") == "test3.zip");
}

BOOST_AUTO_TEST_CASE(testCanSeedBeRolled)
{
    PuzzleFixture a = PuzzleFixture();
    BOOST_ASSERT(rand() != rand());
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    remove("hello.txt");
    remove("congrats.txt");
    remove("congrats.zip");
    remove("test.zip");
    remove("test2.zip");
    remove("test3.zip");
    remove("test4.zip");
}