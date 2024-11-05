cmake_minimum_required(VERSION 3.5)
project(my_crow_project)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Crow as a subdirectory
add_subdirectory(Crow)

# Define the main application
add_executable(pointless_web main.cpp)

# Link the Crow library to the application
target_link_libraries(pointless_web Crow::Crow)
