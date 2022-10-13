#include "../util/Puzzle.h"
#include "../util/HtmlGenerator.h"
class HelloWorld : Puzzle 
{
protected:
    void setAnswer()
    {
        answer = "4";
    }
    std::vector<std::string> generateFiles(std::string containedZipPath)
    {
        std::string file = generateHtml("Hello, World!", "What is 2+2?", rollSeed());
        return {containedZipPath, file};
    }
};