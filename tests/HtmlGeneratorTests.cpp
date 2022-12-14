#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HtmlGeneratorTests
#include <boost/test/unit_test.hpp>
#include <stdio.h>
#include <fstream>
#include "../util/HtmlGenerator.h"

BOOST_AUTO_TEST_CASE(testCanGenerateBeCalled) 
{
    chdir("puzzles");
    try 
    {
        BOOST_ASSERT(generateHtml("title", "test", 0) == "title.html");
        BOOST_ASSERT(remove("title.html") == 0);
    } 
    catch(std::exception e)
    {
        std::cerr << e.what();
    }
}

BOOST_AUTO_TEST_CASE(testDoesGeneratePopulateHtmlFile)
{
    try 
    {
        std::string filePath = generateHtml("test", "abc", 0);
        std::ifstream file = std::ifstream(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        BOOST_ASSERT(buffer.str().length() >= 7);
        BOOST_ASSERT(remove(filePath.c_str()) == 0);    
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }
}

BOOST_AUTO_TEST_CASE(testDoesGeneratePutTitleInHtmlFile)
{
    try 
    {
        std::string filePath = generateHtml("test", "abc", 0);
        std::ifstream file = std::ifstream(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        BOOST_ASSERT(buffer.str().find("test") != std::string::npos);
        BOOST_ASSERT(remove(filePath.c_str()) == 0);    
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }

}

BOOST_AUTO_TEST_CASE(testDoesGeneratePutDescriptionInHtmlFile)
{
    try 
    {
        std::string filePath = generateHtml("test", "abc", 0);
        std::ifstream file = std::ifstream(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        BOOST_ASSERT(buffer.str().find("abc") != std::string::npos);
        BOOST_ASSERT(remove(filePath.c_str()) == 0);    
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }
}

BOOST_AUTO_TEST_CASE(testDoesGeneratePutSeedInHtmlFile)
{
    try 
    {
        std::string filePath = generateHtml("test", "abc", 6578293);
        std::ifstream file = std::ifstream(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        BOOST_ASSERT(buffer.str().find("6578293") != std::string::npos);
        BOOST_ASSERT(remove(filePath.c_str()) == 0);    
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }
}