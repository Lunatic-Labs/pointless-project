#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

typedef std::vector<std::string> strvec_t;

void generate_file(std::string output_filepath, std::string output_body);
int utils_rng_roll(int min, int max, long seed);
strvec_t utils_walkdir(const std::string& path);
void utils_zip_files(std::string out_file_name, strvec_t file_names, std::string password="");

#endif // UTILS_H
