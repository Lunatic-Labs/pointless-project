#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#define BOLD(s) "<b>" + (s) + "</b>"

typedef std::vector<std::string> strvec_t;
typedef const std::string filepath_t;

void utils_generate_file(filepath_t, std::string output_body);
int utils_rng_roll(int min, int max, long &seed);
int utils_roll_seed(void);
strvec_t utils_walkdir(filepath_t path);
void utils_zip_files(std::string out_file_name, strvec_t file_names, std::string password="");
std::string utils_file_to_str(filepath_t filepath);
std::string utils_html_printf(std::string title, filepath_t desc_filepath, strvec_t args);
void utils_mkdir(filepath_t filepath);

#endif // UTILS_H
