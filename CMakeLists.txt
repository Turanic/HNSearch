cmake_minimum_required(VERSION 3.9)
project(hnStat CXX)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Werror")
set(CMAKE_CXX_FLAG_DEBUG "-O0 -ggdb3 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()
endif()

# sources/exe dirs
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
