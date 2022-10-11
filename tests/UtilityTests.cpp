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
    std::vector<std::string> fileNames;
    FileStructureFixture() 
    {
        fileNames = {"abc.txt"};
        std::ofstream writer = std::ofstream(fileNames[0], std::ios::out);
        writer << "abc";
        writer.flush();
        writer.close();
    }
    ~FileStructureFixture() {
        remove("abc.txt");
    }
};

BOOST_FIXTURE_TEST_CASE(testZipperDoesCreateZipFile, FileStructureFixture) 
{
    zipFiles("test.zip", fileNames);
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_FIXTURE_TEST_CASE(testZipperDoesCreateZipFileWithPassword, FileStructureFixture) 
{
    zipFiles("test.zip", fileNames, "abcdef");
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_FIXTURE_TEST_CASE(testZipperDoesWriteFileToZipFile, FileStructureFixture) 
{
    zipFiles("test.zip", fileNames);
    int * err = nullptr;
    zip_t * archive = zip_open("test.zip", 0, err);
    
    BOOST_ASSERT(zip_fopen(archive, fileNames[0].c_str(), 0) != nullptr);
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_FIXTURE_TEST_CASE(testZipperDoesWriteEncryptedFileToZipFile, FileStructureFixture) 
{
    zipFiles("test.zip", fileNames, "abc");
    int * err = nullptr;
    zip_t * archive = zip_open("test.zip", 0, err);
    
    BOOST_ASSERT(zip_fopen_encrypted(archive, fileNames[0].c_str(), 0, "abc") != nullptr);
    BOOST_ASSERT(zip_fopen(archive, fileNames[0].c_str(), 0) == nullptr);
    BOOST_ASSERT(remove("test.zip") == 0);
}

BOOST_AUTO_TEST_CASE(testZipperCanWriteMultipleFiles) 
{
    std::vector<std::string> fileNames;
    for(int i = 0; i < 26; ++i) 
    {
        fileNames.push_back("test_file_");
        fileNames[i] += ('a' + i);
        std::ofstream writer = std::ofstream(fileNames[i], std::ios::out);
        writer << "test";
        writer.flush();
        writer.close();
    }

    zipFiles("test.zip", fileNames);
    int err = 0;
    zip_t * archive = zip_open("test.zip", 0, &err);
    BOOST_ASSERT(archive);
    
    for(std::string fileName : fileNames) 
    {
        zip_file_t * file = zip_fopen(archive, fileName.c_str(), 0);
        BOOST_ASSERT(zip_fclose(file) == 0);
    }

    for(std::string fileName : fileNames)
        remove(fileName.c_str());
    
    remove("test.zip");
}