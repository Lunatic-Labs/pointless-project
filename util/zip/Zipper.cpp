#include <vector>
#include <iostream>
#include <zip.h>
#include <cassert>


#ifndef ZIP
#define ZIP


// Zips every file in the vector and saves it to the output file
void zipFiles(std::string outFileName, std::vector<std::string> fileNames)
{
    int * err = nullptr;
    zip * zipFile = zip_open(outFileName.c_str(), ZIP_CREATE, nullptr);
    assert(!err);
    for (auto fileName : fileNames) 
    {
        zip_source_t * src = zip_source_file(zipFile, fileName.c_str(), 0, 0);

        zip_add(zipFile, fileName.c_str(), src); 
    }
    zip_close(zipFile);
}
#endif

int main()
{
    std::string zipName;
    std::cin >> zipName;

    std::vector<std::string> fileNames;
    std::string next;
    while (std::cin >> next)
    {
        fileNames.push_back(next);
    }
    zipFiles(zipName, fileNames);
}