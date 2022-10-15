#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EntryPointTests
#include <boost/test/unit_test.hpp>
#include "../PuzzleGen.cpp"

BOOST_AUTO_TEST_CASE(testCanInitializeBeCalled)
{
    const char * inp [] = {"./Program"};
    int argc = 0;
    initialize(argc, inp);
}