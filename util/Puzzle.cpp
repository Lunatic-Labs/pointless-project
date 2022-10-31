#include <string>
#include <vector>
#include "Puzzle.h"
#include "zip/Zipper.h"
#include <iostream>
#include <unistd.h>

int Puzzle::rollSeed() 
{
    long seed = (uint) time(nullptr); 
    srand(seed);
    return seed;
}

/**
 * Initializes the puzzle, returning the path of the zip containing the puzzle.
 * @param puzzleName: The name of the puzzle
 * @param containedZipPath: The name of the Zip of the previous puzzle, 
 * or congratulatory Zip
 * 
 * @param prevAnswer: The answer to the previous puzzle
 **/
std::string Puzzle::init(std::string puzzleName, 
                         std::string containedZipPath,
                         int seed)
{

    this->seed = (seed == -1) ? rollSeed() : seed;
    setAnswer();

    name = puzzleName;

    std::vector<std::string> outFiles = generateFiles(containedZipPath);
    
    try
    {
        zipFiles(puzzleName + ".zip", outFiles, answer);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    for(int i = 0; i < outFiles.size(); ++i)
    {
        if(outFiles[i] != "congrats.zip")
            remove(outFiles[i].c_str());
    }
    return puzzleName + ".zip";
}