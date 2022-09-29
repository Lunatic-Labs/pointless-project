#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ParameterizedFileGenerationTests
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "../util/files/FileUtils.h"
#include <stdio.h>

BOOST_AUTO_TEST_CASE(testSetups) {
    std::ofstream writer = std::ofstream("testNoParameters.txt", std::ios::out);
    writer << "test";
    writer.close();

    writer = std::ofstream("testOneParameter.txt", std::ios::out);
    writer << "test {param1}";
    writer.close();

    writer = std::ofstream("testMultipleParameters.txt", std::ios::out);
    writer << "test {param1} {param2} {param1}";
    writer.close();
}

BOOST_AUTO_TEST_CASE(testCanParamterizeBeCalled) {
    Parameterize("testNoParameters.txt", "testNoParamsOut.txt");

    std::ifstream reader = std::ifstream("testNoParamsOut.txt", std::ios::in);
    
    std::string base;
    reader >> base;

    reader.close();

    reader = std::ifstream("testNoParameters.txt", std::ios::in);
    std::string compare;
    reader >> compare;

    reader.close();

    BOOST_ASSERT(compare == base);
    BOOST_ASSERT(remove("testNoParamsOut.txt") == 0);
}

BOOST_AUTO_TEST_CASE(cleanup) {
    BOOST_ASSERT(remove("testNoParameters.txt") == 0);
    BOOST_ASSERT(remove("testOneParameter.txt") == 0);
    BOOST_ASSERT(remove("testMultipleParameters.txt") == 0);
}