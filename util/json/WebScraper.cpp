#include "WebScraper.h"
#include <string>
#include <curl/curl.h>
#include <regex>
#include <iostream>
#include "../StringFunctions.cpp"

#define NO_VALUE -1

// Thanks to https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<std::string> GetAttributes(std::string object)
{
    std::vector<std::string> retValue;

    std::string word = "";
    bool isRecording = false;
    for(char c : object)
    {
        if(isRecording) {
            if(c != '"') {
                word += c;
            } else {
                retValue.push_back(word);
                word = "";
                isRecording = false;
            }
        } else if (c == '"') {
            isRecording = true;
        }
    }
    return retValue;
}

Course getCourse(std::string courseString) 
{
    std::vector<std::string> attributes = GetAttributes(courseString);
    Course ret;
    if(attributes.size() > 0)
    {
        ret.courseCode = attributes[0];
        ret.section = attributes[1];
        ret.name = attributes[2];
        ret.term = attributes[3];
        ret.start = attributes[4];
        ret.end = attributes[5];
        ret.instructor = attributes[6];
        ret.startTime = attributes[7];
        ret.endTime = attributes[8];
        ret.meetingDays = attributes[9];
        ret.seats = (attributes[10].length() > 0) ? stoi(attributes[10]) : NO_VALUE;
        ret.filledSeats = (attributes[11].length() > 0) ? stoi(attributes[11]) : NO_VALUE;
        ret.creditHours = (attributes[12].length() > 0) ? atof(attributes[12].c_str()) : NO_VALUE;
    }
    return ret;
}

std::vector<Course> parseData(std::string src, int start, int end)
{
    std::string interestingData = src.substr(start+9,end-start-9);
    std::cout << interestingData << std::endl;
    std::vector<std::string> courseStrings;
    bool recordingData = false;
    std::string curr = "";
    for(char c : interestingData) {
        if(recordingData) {
            if(c == ']') {
                recordingData = false;
                courseStrings.push_back(curr);
                curr = "";
            } else {
                curr += c;
            }
        } else {
            if(c == '[')
                recordingData = true;
        }
    }

    std::vector<Course> returnValue;
    for(auto courseString : courseStrings) {
        returnValue.push_back(getCourse(courseString));
    }
    return returnValue;
}

std::vector<Course> getCourses (std::string year, std::string semester)
{
    std::string url = "http://courseschedule.lipscomb.edu/ScheduleP" + year + semester + ".html";

    std::cout << url << std::endl;

    std::string result = "";
    std::string header;

    CURLcode responseCode;
    
    CURL * getter = curl_easy_init();
    curl_easy_setopt(getter, CURLOPT_URL, url.c_str());
    curl_easy_setopt(getter, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(getter, CURLOPT_HEADERDATA, &header);
    curl_easy_setopt(getter, CURLOPT_WRITEFUNCTION, writeCallback);

    curl_easy_perform(getter);
    curl_easy_cleanup(getter);

    const std::regex startPattern = std::regex(",\"data\":\\[");
    const std::regex endPattern = std::regex("],\"iDisplayLength\":-1,");

    std::smatch startMatches, endMatches;

    int start=-1,end=-1;

    if(std::regex_search(result, startMatches, startPattern)) 
    {
        start = startMatches.position();
    }
    if(std::regex_search(result, endMatches, endPattern)) 
    {
        end = endMatches.position();
    }

    return parseData(result, start, end);
}