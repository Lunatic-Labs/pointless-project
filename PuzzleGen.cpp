#include "util/zip/Zipper.h"
#include <stack>
#include <fstream>
#include "util/Puzzle.h"
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include "puzzles/HelloWorld.cpp"

void registerPuzzles(std::stack<std::pair<std::string, Puzzle*>> & puzzles)
{
    // Puzzles will be registered here
    puzzles.push(std::make_pair(std::string("Hello World"), new HelloWorld()));
} 

void initialize(int argc, char const* const* argv) 
{
    
    std::stack<std::pair<std::string, Puzzle*>> puzzles;
    registerPuzzles(puzzles);
    if (argc == 1)
    {
        std::string prevFile = "congrats.zip";
        while(!puzzles.empty())
        {
            std::pair<std::string, Puzzle*> puzzle = puzzles.top();
            puzzles.pop();
            prevFile = puzzle.second->init(puzzle.first,
                                           prevFile);
            delete puzzle.second;
        }
    }

    // ./Program <Puzzle> [seed]
    // Runs program to build Puzzle with seed Seed, if any is supplied
    if (argc == 2) 
    {
        std::string prevFile = "End.zip";
        for(int i = 0; !(puzzles.empty()) && i < std::stoi(argv[1]); ++i)
            puzzles.pop();
        puzzles.top().second->init(puzzles.top().first, prevFile);
    }

    if (argc == 3) 
    {
        std::string prevFile = "End.zip";
        for(int i = 0; i < std::stoi(argv[1]); ++i) 
            puzzles.pop();
        
        puzzles.top().second->init(puzzles.top().first, 
                                   prevFile, 
                                   std::stoi(argv[2]));
    }

    // Invalid Syntax
    if (argc > 2)
    {
        std::cout << "Usage: ./Program <PuzzleNo> [seed]";
    }
}