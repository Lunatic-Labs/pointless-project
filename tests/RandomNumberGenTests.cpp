#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RandomNumberGenTests
#include <boost/test/unit_test.hpp>
#include "../util/rng/RandomNumber.cpp"

BOOST_AUTO_TEST_CASE(testGeneratorWorksWithLargeNumbers) 
{
    int number = getNum(-1000000, 1000000);
    BOOST_ASSERT(number >= -1000000);
    BOOST_ASSERT(number <= 1000000);
}

BOOST_AUTO_TEST_CASE(testGeneratorWorksWithSmallNumbers) 
{
    int number = getNum(0, 5);
    BOOST_ASSERT(number >= 0);
    BOOST_ASSERT(number <= 5);
}

BOOST_AUTO_TEST_CASE(testGeneratorWorksWithLargeNumbersInversed) 
{
    int number = getNum(1000000, -1000000);
    BOOST_ASSERT(number >= -1000000);
    BOOST_ASSERT(number <= 1000000);
}

BOOST_AUTO_TEST_CASE(testGeneratorWorksWithSmallNumbersInversed) 
{
    int number = getNum(5, 0);
    BOOST_ASSERT(number >= 0);
    BOOST_ASSERT(number <= 5);
}