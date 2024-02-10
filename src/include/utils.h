#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#define BOLD(s) "<b>" + (s) + "</b>"

typedef std::vector<std::string> strvec_t;
typedef const std::string filepath_t;

// Generates a file with the given body. Will be written to the given filepath.
void utils_generate_file(filepath_t filepath, std::string output_body);

// Generates a random number between `min` and `max` using the given seed.
// NOTE: The seed will be modified.
int utils_rng_roll(int min, int max, long &seed);

// Rolls a seed using the current time.
int utils_roll_seed(void);

// Returns a vector of strings containing the names of all files in the given directory.
// Recursively walks all subdirectories.
strvec_t utils_walkdir(filepath_t path);

// Zips the given files into a single file with the given name and password.
void utils_zip_files(filepath_t out_file_name, strvec_t file_names, std::string password="");

// Returns the contents of the given file as a string.
std::string utils_file_to_str(filepath_t filepath);

// Creates an HTML body. All occurrences of "%DELIM" in the text of `desc_filepath` will be 
// replaced with the given `args` in order. It is similar to `printf`.
std::string utils_html_printf(std::string title, filepath_t desc_filepath, strvec_t args);

#endif // UTILS_H
