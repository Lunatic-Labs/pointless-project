#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EntryPointTests
#include <boost/test/unit_test.hpp>
#include "../PuzzleGen.cpp"

BOOST_AUTO_TEST_CASE(testCanInitializeBeCalled)
{
    const char * inp[1];
    const char * str = "./Program";
    inp[0] = str;
    int argc = 1;
    initialize(argc, inp);
}