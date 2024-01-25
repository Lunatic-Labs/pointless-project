#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RandomNumberGenTests
#include <boost/test/unit_test.hpp>
#include "../util/rng/RandomNumber.h"

BOOST_AUTO_TEST_CASE(test_generator_works_with_large_numbers) 
{
    int number = get_num(-1000000, 1000000);
    BOOST_ASSERT(number >= -1000000);
    BOOST_ASSERT(number <= 1000000);
}

BOOST_AUTO_TEST_CASE(test_generator_works_with_small_numbers) 
{
    int number = get_num(0, 5);
    BOOST_ASSERT(number >= 0);
    BOOST_ASSERT(number <= 5);
}

BOOST_AUTO_TEST_CASE(test_generator_works_with_large_numbers_inversed) 
{
    int number = get_num(1000000, -1000000);
    BOOST_ASSERT(number >= -1000000);
    BOOST_ASSERT(number <= 1000000);
}

BOOST_AUTO_TEST_CASE(test_generator_works_with_small_numbers_inversed) 
{
    int number = get_num(5, 0);
    BOOST_ASSERT(number >= 0);
    BOOST_ASSERT(number <= 5);
}
