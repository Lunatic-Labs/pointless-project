#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE WebScraperTests
#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include "../util/json/WebScraper.h"

// TODO (jwroberts1@mail.lipscomb.edu): Write more tests
BOOST_AUTO_TEST_CASE(testDoesGetCourseFunctionWorkProperly)
{
    std::string courseString = "\"AM4713\",\"16\",\"Computer Animation I\",\"2022-01-10\",\"2022-05-04\",\"Stars, Eric\",\"08:00\",\"10:40\",\"TuesThurs\",\"16\",\"6\",\"10\",\"\",\"On Ground\",\"Shinn 105\",\"\"";
    Course course = getCourse(courseString);
    std::cout << course.courseCode << std::endl;
    BOOST_ASSERT(course.courseCode == "AM4713");
    BOOST_ASSERT(course.section == "16");
    std::cout << course.name << std::endl;
    BOOST_ASSERT(course.name == "Computer Animation I");
    
    // For now, it's sufficient to test if we *can* do this
    getCourses("2023", "SPRING");
}