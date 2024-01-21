#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

int get_num(int min, int max, int seed=-1);

std::vector<std::string> walkdir(const std::string& path);

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

#endif // UTILS_H