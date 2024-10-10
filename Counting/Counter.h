//
// Created by Piotr on 09.10.2024.
//

#ifndef COUNTER_H
#define COUNTER_H
#include "../Graph/Node.h"
#include <vector>



class Counter {
public:
    static double relativeErrorSum(std::vector<double> times, double avg_time);
    static double absoluteErrorSum(std::vector<double> times, double avg_time);
    void printGraph(std::vector<Node> graph, int V);//metoda do printowania grafu
};



#endif
