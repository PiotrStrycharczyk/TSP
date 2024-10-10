cmake_minimum_required(VERSION 3.28)
project(tsp_problem)

set(CMAKE_CXX_STANDARD 17)

add_executable(tsp_problem Main/main.cpp
        Graph/Node.cpp
        Graph/Node.h
        Graph/Edge.cpp
        Graph/Edge.h
        Main/File.cpp
        Algorithms/TSP.cpp
        Algorithms/TSP.h
        Main/Navigator.cpp
        Main/Navigator.h
        Counting/Counter.cpp
        Counting/Counter.h)
