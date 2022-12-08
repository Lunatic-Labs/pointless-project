#include <vector>
#include <string>


std::vector<std::string> splitByDelimiter(const std::string & line, char delimiter=' ')
{
    std::vector<std::string> words;

    std::string word = "";
    for(int i = 0; i < line.length(); ++i)
    {
        if(line[i] != delimiter)
            word = word + line[i];
        else
        {
            words.push_back(word);
            word = "";
        }
    }
    words.push_back(word);
    return words;
}