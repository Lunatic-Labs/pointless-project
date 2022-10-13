#include <string>
#include <vector>

#ifndef PUZZLE_H
#define PUZZLE_H
class Puzzle 
{
protected:
    std::string answer;
    int seed;
    std::string name;

    // Methods only should be called once in init()
    int rollSeed();
    virtual void setAnswer() { };
    virtual std::vector<std::string> generateFiles(std::string containedZipPath) { return {};};
public:
    std::string init(std::string puzzleName, 
                     std::string containedZipPath,
                     int seed=-1);

    std::string getAnswer();

    int getSeed() { srand(seed+1); return seed++; }
};

#endif