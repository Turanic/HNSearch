cmake_minimum_required(VERSION 3.9)
project(hnStat CXX)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

enable_testing()

# sources/exe dirs
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
