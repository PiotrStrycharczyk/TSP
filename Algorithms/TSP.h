//
// Created by Piotr on 07.10.2024.
//

#ifndef TSP_H
#define TSP_H

#include <vector>
#include "../Graph/Node.h"
#include "../Counting/Timer.h"

class TSP {
public:
    TSP(Timer& timer, int shortest_path_from_file);
    int bruteForce(std::vector<Node> graph, int V);
    int repetetiveNearestNeighbour(std::vector<Node> graph, int V);
    int nearestNeighbour(std::vector<Node> graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost);

    int randomMetod(std::vector<Node> graph, int V);
    std::vector<int> getSolvedPath();
    int getShortestPath();
    bool IfEndedWithIterations();

private:
    Timer& timer_;
    bool if_ended_by_iterations = true;//zakldamy ze konczy sie przez iteracje ale gdy jednak przez czas albo znalezienie dobrej odp to zwracamy false
    int shortestpath;
    std::vector<int> solvedpath;
    bool ifAllVisited(std::vector<bool> odwiedzone);
    int shortest_path_from_file;
};



#endif //TSP_H
