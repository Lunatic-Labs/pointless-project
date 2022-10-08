#include <vector>
#include <iostream>
#include <zip.h>
#include <cassert>


#ifndef ZIP_H
#define ZIP_H

/**
 * Creates new zip file with outFileName if not exists and compresses every file
 * in fileNames, setting the password of the first file added.
 * @param outFileName: Relative path of zip file to write to
 * @param fileNames: Vector of relative paths to files to compress
 * @param password: Desired password of first file added
 **/
void zipFiles(std::string outFileName, std::vector<std::string> fileNames, std::string password="")
{
    int * err = nullptr;
    zip * zipFile = zip_open(outFileName.c_str(), ZIP_CREATE, nullptr);
    assert(!err);
    
    for (int i = 0; i < fileNames.size(); ++i) 
    {
        zip_source_t * src = zip_source_file(zipFile, fileNames[i].c_str(), 0, 0);
        zip_add(zipFile, fileNames[i].c_str(), src);

        // Encrypt first file added to the zip file
        if(i == 0 && password != "")
            zip_file_set_encryption(zipFile, 0, ZIP_EM_AES_256, password.c_str());
    }
    zip_close(zipFile);
}
#endif