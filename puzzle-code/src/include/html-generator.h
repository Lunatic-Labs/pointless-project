#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef HTML_GENERATOR_H
#define HTML_GENERATOR_H

// Generates a simple HTML file with a title, a description and a random seed.
std::string generate_html(std::string title, std::string description, long seed);

#endif
