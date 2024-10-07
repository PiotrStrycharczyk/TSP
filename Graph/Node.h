//
// Created by Piotr on 04.10.2024.
//

#ifndef NODE_H
#define NODE_H
#include <utility>
#include <vector>

#include "Edge.h"


class Node {
public:
    Node(int number);

    int num;
    std::vector<Edge> edges;
    void addEdge(int dir, int weight);//dodawanie krawedzi o danych wagach

};



#endif //NODE_H
