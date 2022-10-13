#include "HtmlGenerator.h"

std::string generateHtml(std::string title, std::string description, long seed)
{
    // Stores the content of templateHeader into a std::string. 
    char c;
    std::string headerContent;
    std::ifstream headerFile( "templateHeader.txt" );
    headerFile >> std::noskipws;
    while ( headerFile >> c )  headerContent += c;

    headerFile.close();
    // Stores the content of templateFooter into a std::string. 
    char d;
    std::string footerContent;
    std::ifstream footerFile( "templateFooter.txt" );
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