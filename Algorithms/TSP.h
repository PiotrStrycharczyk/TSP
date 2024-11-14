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
    int repetetiveNearestNeighbour(std::vector<Node> graph, int V, std::vector<bool>& odwiedzone,
        int current_cost, std::vector<int>& current_path, std::vector<int>& best_path, int& best_cost);
    int nearestNeighbour(std::vector<Node> graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost);
    bool ifAllVisited(std::vector<bool> odwiedzone);
    int randomMetod(std::vector<Node> graph, int V);
    std::vector<int> getSolvedPath();
    int getShortestPath();

private:
    int shortestpath;
    std::vector<int> solvedpath;

};



#endif //TSP_H
