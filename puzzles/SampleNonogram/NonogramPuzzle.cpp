#include "../../util/Puzzle.h"
#include "../../util/HtmlGenerator.h"
#include <vector>

class ColorPuzzle : public Puzzle {
public:
    void setAnswer() override
    {
        
    }
    virtual std::vector<std::string> generateFiles(std::string containedZipPath) override
    {
        std::ifstream descFile = std::ifstream("SampleNonogram/desc.txt");
        std::stringstream descStream;
        descStream << descFile.rdbuf();
        std::string desc = descStream.str();

        descFile.close();
        std::string file = generateHtml("Read.Me", desc, 0);
        return {containedZipPath, file};
    }
};