#include <vector>
#include <iostream>
#include <zip.h>
#include <cassert>

#ifndef ZIP_H
#define ZIP_H

/**
 * Creates new zip file with out_file_name if not exists and compresses every file
 * in file_names, setting the password of the first file added.
 * @param out_file_name: Relative path of zip file to write to
 * @param file_names: Vector of relative paths to files to compress
 * @param password: Desired password of first file added
 **/
void zip_files(std::string out_file_name,
              std::vector<std::string> file_names,
              std::string password="");
#endif // ZIP_H

