#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EntryPointTests
#include <boost/test/unit_test.hpp>
#include "../main.cpp"

BOOST_AUTO_TEST_CASE(test_can_initialize_be_called)
{
    const char * inp[1];
    const char * str = "./Program";
    inp[0] = str;
    int argc = 1;
    initialize(argc, inp);
}

