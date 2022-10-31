#include <vector>
#include <iostream>
#include <zip.h>
#include <cassert>
#include <sys/stat.h>

void zipFiles(std::string outFileName, 
              std::vector<std::string> fileNames, 
              std::string password)
{
    int * err = nullptr;
    zip * zipFile = zip_open(outFileName.c_str(), ZIP_CREATE, nullptr);
    assert(!err);
    
    for (int i = 0; i < fileNames.size(); ++i) 
    {

        struct stat buffer;
        if (stat(fileNames[i].c_str(), &buffer) == 0) // File not exists
            throw(std::runtime_error("Could not open file: File does not exist"));
        
        zip_source_t * src = zip_source_file(zipFile, fileNames[i].c_str(), 0, 0);
        zip_add(zipFile, fileNames[i].c_str(), src);
        // Encrypt first file added to the zip file
        if(i == 0 && password != "")
            zip_file_set_encryption(zipFile, 0, ZIP_EM_AES_256, password.c_str());
    }
    zip_close(zipFile);
}
