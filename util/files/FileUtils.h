#include <string>
#include <fstream>

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

void parameterize(std::string originPath, std::string outPath, std::string * params = nullptr, int n=0) {
    
    std::ifstream inFile = std::ifstream(originPath, std::ios::in);
    std::string base;
    inFile >> base;
    inFile.close();

    std::ofstream outFile = std::ofstream(outPath, std::ios::out);
    outFile << base;
    outFile.close();
}
#endif