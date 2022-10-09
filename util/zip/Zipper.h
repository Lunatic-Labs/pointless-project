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
void zipFiles(std::string outFileName, 
              std::vector<std::string> fileNames, 
              std::string password="");
#endif