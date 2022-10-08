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
    virtual std::vector<std::string> generateFiles(std::string containedZipPath) { };
public:
    std::string init(std::string puzzleName, 
                     std::string containedZipPath,
                     int seed);

    std::string getAnswer();

    int getseed() { return seed; }
};

#endif