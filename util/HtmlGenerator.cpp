#include "HtmlGenerator.h"
#include <sys/stat.h>

std::string generateHtml(std::string title, std::string description, long seed)
{
    // Stores the content of templateHeader into a std::string. 
    char c;
    std::string headerContent;
    std::string headerPath = "resources/templateHeader.txt";

    struct stat buf;
    if(stat(headerPath.c_str(), &buf) != 0)
        throw("Can not open " + headerPath + ": No such file");

    std::ifstream headerFile( headerPath );

    headerFile >> std::noskipws;
    while ( headerFile >> c )  headerContent += c;

    headerFile.close();
    // Stores the content of templateFooter into a std::string. 
    char d;
    std::string footerContent;

    std::string footerPath = "resources/templateFooter.txt";

    if(stat(footerPath.c_str(), &buf) != 0)
        throw("Can not open " + footerPath + ": No such file");

    std::ifstream footerFile( footerPath );
    footerFile >> std::noskipws;
    while ( footerFile >> d ) footerContent += d;
    footerFile.close();

    std::string contentConcatenated = headerContent 
                                      + "<h3 style=\"text-align:center\">" 
                                      + title 
                                      + "</h3>" 
                                      + "<p style=\"text-align:center\">" 
                                      + description 
                                      + "</p>" 
                                      + footerContent;
    
    std::string templateSeed = std::to_string(seed) 
                               + "</div>\n" 
                               + "</div>\n" 
                               + "</div>\n" 
                               + "</body>\n" 
                               + "</html>\n";

  
    std::string fileName =  title + ".html";

    //Creates and inserts content into a new file with the title as name.html
    std::ofstream outfile (fileName);
    outfile << contentConcatenated;
    outfile << templateSeed;
    outfile.close();

    return fileName;
};