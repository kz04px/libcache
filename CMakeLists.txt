cmake_minimum_required(VERSION 3.10)

# Project
project(libcache VERSION 1.0 LANGUAGES CXX)

# Find packages
find_package(Catch2 CONFIG REQUIRED)

# Base directory relative includes
include_directories(./src)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the test executable
add_executable(
    libcache-test
    tests/main.cpp
    tests/lru/constructor.cpp
    tests/lru/insert.cpp
    tests/lru/use.cpp
)

set_property(TARGET libcache-test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)

target_link_libraries(libcache-test Catch2::Catch2)
