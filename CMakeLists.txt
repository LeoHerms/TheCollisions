cmake_minimum_required(VERSION 3.29)
project(Collisions)

set(CMAKE_CXX_STANDARD 20)

add_executable(Collisions main.cpp
        aStarAlgorithm.cpp
        aStarAlgorithm.h)
