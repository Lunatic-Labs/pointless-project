#include <string>

class Puzzle 
{
public:
    std::string init();
    std::string init(long seed);
    std::string getAnswer();
    std::string getseed();
};