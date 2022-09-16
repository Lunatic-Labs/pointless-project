#include "boost/iostreams/filtering_streambuf.hpp"
#include "boost/iostreams/filter/zlib.hpp"
#include "boost/iostreams/copy.hpp"
#include <fstream>
#include <vector>
#include <iostream>

#ifndef ZIP
#define ZIP

// Zips every file in the vector and saves it to the output file
void zipFiles(std::vector<std::string> input, std::string output)
{
    std::ofstream zip = std::ofstream(output, std::ios_base::out | std::ios_base::binary);
    boost::iostreams::filtering_streambuf<boost::iostreams::output> out;

    for(auto s : input) {
            std::ifstream file = std::ifstream(s, std::ios_base::in | std::ios_base::binary);
            
            out.push(boost::iostreams::zlib_compressor());
            out.push(file);
    }
    boost::iostreams::copy(out, zip);
}
#endif

int main()
{
    std::vector<std::string> files;
    std::string s;
    while(std::cin >> s) {
        files.push_back(s);
    }
    zipFiles(files, "output.zip"); 
}