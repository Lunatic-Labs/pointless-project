#include "../../util/Puzzle.h"
#include "../../util/HtmlGenerator.h"
#include <iostream>
class HelloWorld : public Puzzle
{
protected:
    void setAnswer() override
    {
        answer = "4";
    }
    virtual std::vector<std::string> generateFiles(std::string containedZipPath) override
    {
        try 
        {
            std::string file = generateHtml(name, "What is 2+2?", getSeed());
            return {containedZipPath, file};
        }
        catch(std::exception e)
        {
            std::cerr << e.what();
            return {};
        }
    }
};