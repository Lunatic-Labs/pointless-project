#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UtilityTests
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <zip.h>
#include "../util/zip/Zipper.h"

struct FileStructureFixture 
{
    std::vector<std::string> file_names;
    FileStructureFixture() 
    {
        file_names = {"abc.txt"};
        std::ofstream writer = std::ofstream(file_names[0], std::ios::out);
        writer << "abc";
        writer.flush();
        writer.close();
    }
    ~FileStructureFixture() {
        remove("abc.txt");
    }
};

BOOST_FIXTURE_TEST_CASE(test_zipper_does_create_zip_file, FileStructureFixture) 
{
    try
    {
        zip_files("test.zip", file_names);
        BOOST_ASSERT(remove("test.zip") == 0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        BOOST_ASSERT(false);
    }
}

BOOST_FIXTURE_TEST_CASE(test_zipper_does_create_zip_file_with_password, FileStructureFixture) 
{
    try 
    {
        zip_files("test.zip", file_names, "abcdef");
    }
    catch(const std::exception & err)
    {
        std::cerr << err.what() << "\n";
    }
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_FIXTURE_TEST_CASE(test_zipper_does_write_file_to_zip_file, FileStructureFixture) 
{
    try 
    {
        zip_files("test.zip", file_names);
    }
    catch(const std::exception & err)
    {
        std::cerr << err.what() << "\n";
    }
    int * err = nullptr;
    zip_t * archive = zip_open("test.zip", 0, err);
    
    BOOST_ASSERT(zip_fopen(archive, file_names[0].c_str(), 0) != nullptr);
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_FIXTURE_TEST_CASE(test_zipper_does_write_encrypted_file_to_zip_file, FileStructureFixture) 
{
    try 
    {
        zip_files("test.zip", file_names, "abc");
    }
    catch(const std::exception & err)
    {
        std::cerr << err.what() << "\n";
    }
    
    int * err = nullptr;
    zip_t * archive = zip_open("test.zip", 0, err);
    
    BOOST_ASSERT(zip_fopen_encrypted(archive, file_names[0].c_str(), 0, "abc") != nullptr);
    BOOST_ASSERT(zip_fopen(archive, file_names[0].c_str(), 0) == nullptr);
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_AUTO_TEST_CASE(test_zipper_can_write_multiple_files) 
{
    std::vector<std::string> file_names;
    for(int i = 0; i < 26; ++i) 
    {
        file_names.push_back("test_file_");
        file_names[i] += ('a' + i);
        std::ofstream writer = std::ofstream(file_names[i], std::ios::out);
        writer << "test";
        writer.flush();
        writer.close();
    }

    try 
    {
        zip_files("test.zip", file_names);
    }
    catch(const std::exception & err)
    {
        std::cerr << err.what() << "\n";
    }
    int err = 0;
    zip_t * archive = zip_open("test.zip", 0, &err);
    BOOST_ASSERT(archive);
    
    for(std::string file_name : file_names) 
    {
        zip_file_t * file = zip_fopen(archive, file_name.c_str(), 0);
        BOOST_ASSERT(zip_fclose(file) == 0);
    }

    for(std::string file_name : file_names)
        remove(file_name.c_str());
    
    remove("test.zip");
}
