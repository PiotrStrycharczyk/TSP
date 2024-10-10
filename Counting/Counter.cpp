//
// Created by Piotr on 09.10.2024.
//

#include "Counter.h"
#include <iostream>
#include <cmath>


void Counter::printTimes(std::vector<double> times, double avg_time) {
    int n = times.size();
    double sum_relative = 0;
    double sum_absolute = 0 ;
    std::cout<<"LP;czas;blad bezwzgledny;blad wzgledny"<<std::endl;
    for (int i = 0 ; i < n ; i++) {
        double relative_error = fabs (times[i] - avg_time)/times[i];
        sum_relative += relative_error;
        double absoluteerror = fabs (times[i] - avg_time);
        sum_absolute += absoluteerror;
        std::cout << i+1 << ";" <<times[i] << "ms;" << relative_error <<";" <<absoluteerror <<"ms"<< std::endl;

    }
    double avg_rel_error;
    double avg_abs_error;
    avg_rel_error = sum_relative/n;
    avg_abs_error = sum_absolute/n;

    std::cout<<"Czas sredni;bl wzgledny;bl wzgledny[%];bl bezwzgledny;bl bezwgledny[%]" << std::endl;
    std::cout<<avg_time<<"ms:"<< avg_rel_error<<";"<< avg_rel_error * 100<<"%;"<<avg_abs_error << "ms;" << avg_abs_error * 100<<"%"<<std::endl;

}

void Counter::printGraph(std::vector<Node> graph, int V) {
    for(int i = 0 ; i < V ; i++) {//wiersze
        for (int j = 0 ; j < V ; j++) {//kolumny
            std::cout << graph[i].edges[j].weight << " ";
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl;
}
