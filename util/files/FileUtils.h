#include <string>
#include <fstream>

void Parameterize(std::string originPath, std::string outPath, std::string * params = nullptr) {
    std::ifstream inFile = std::ifstream(originPath, std::ios::in);
    std::string base;
    inFile >> base;
    inFile.close();

    std::ofstream outFile = std::ofstream(outPath, std::ios::out);
    outFile << base;
    outFile.close();
}