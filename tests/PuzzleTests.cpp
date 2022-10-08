#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PuzzleTests
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "../util/Puzzle.h"
#include <vector>
#include <string>


class PuzzleFixture : public Puzzle {
protected:
    void setAnswer() 
    {
        answer = "abc123";
    }
    std::vector<std::string> generateFiles(std::string containedZipPath)
    {
        std::ofstream out("hello.txt");
        out << "Hi! The seed to this puzzle is " + seed;
        out.close();
        return {"hello.txt"};
    }
};

BOOST_FIXTURE_TEST_CASE(testCanPuzzleBeInitialized, PuzzleFixture) 
{
    PuzzleFixture a = PuzzleFixture();
    BOOST_ASSERT(a.init("Test", "", 0) == "Test.zip");
}
