//
// Created by Piotr on 09.10.2024.
//

#include "Counter.h"

#include <cmath>
#include <iostream>

void Counter::printGraph(std::vector<Node> graph, int V) {
    for(int i = 0 ; i < V ; i++) {//wiersze
        for (int j = 0 ; j < V ; j++) {//kolumny
            std::cout << graph[i].edges[j].weight << " ";
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl;
}

double Counter::absoluteErrorSum(std::vector<double> times, double avg_time) {
    int n = times.size();
    double sumabs = 0;
    for(int i = 0 ; i < n ;  i++) {
        sumabs += fabs(times[i] - avg_time);
    }
    return sumabs;
}

double Counter::relativeErrorSum(std::vector<double> times, double avg_time) {
    int n = times.size();
    double sumrel = 0;
    for(int i = 0 ; i < n ;  i++) {
        sumrel += fabs(times[i] - avg_time)/times[i];
    }
    return sumrel;
}


