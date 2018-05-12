cmake_minimum_required(VERSION 3.10)
project(assignment)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(assignment
        engine.cpp
        generator.cpp
        tinyxml2.cpp
        tinyxml2.h)
