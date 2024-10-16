//
// Created by Piotr on 07.10.2024.
//

#ifndef TSP_H
#define TSP_H

#include <vector>
#include "../Graph/Node.h"



class TSP {
public:
    TSP();
    int bruteForce(std::vector<Node> graph, int V);
    int repetetiveNearestNeighbour(std::vector<Node> graph, int V);
    int nearestNeighbour(std::vector<Node> graph, int V, int start);
    int randomMetod(std::vector<Node> graph, int V);
    void randomMetodIterations(std::vector<Node> graph, int V, int iter_rand);
    std::vector<int> getSolvedPath();
    int getShortestPath();

private:
    int shortestpath;
    std::vector<int> solvedpath;

};



#endif //TSP_H
