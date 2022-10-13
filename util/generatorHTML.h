#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

std::string generateHtml(std::string title, std::string description, long seed)
{
       // Stores the content of templateHeader into a string. 
    char c;
    string headerContent;
    ifstream headerFile( "templateHeader.txt" );
    headerFile >> noskipws;
    while ( headerFile >> c )  headerContent += c;

    // Stores the content of templateFooter into a string. 
    char d;
    string footerContent;
    ifstream footerFile( "templateFooter.txt" );
    footerFile >> noskipws;
    while ( footerFile >> d ) footerContent += d;


    string contentConcatenated = headerContent + "<h3 style=\"text-align:center\">" + title + "</h3>" + "<p style=\"text-align:center\">" + description + "</p>" + footerContent;
    string templateSeed = to_string(seed) + "</div>\n" + "</div>\n" + "</div>\n" + "</body>\n" + "</html>\n";

    headerFile.close();
    footerFile.close();
  
    string fileName =  title + ".html";

    //Creates and inserts content into a new file with the title as name.html
    std::ofstream outfile (fileName);
    outfile << contentConcatenated;
    outfile << templateSeed;
    outfile.close();

    return fileName;
};