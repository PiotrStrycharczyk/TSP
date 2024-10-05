cmake_minimum_required(VERSION 3.28)
project(tsp_problem)

set(CMAKE_CXX_STANDARD 17)

add_executable(tsp_problem main.cpp
        Node.cpp
        Node.h
        Edge.cpp
        Edge.h
        File.cpp)
