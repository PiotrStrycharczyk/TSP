//
// Created by Piotr on 04.10.2024.
//

#include "Node.h"


Node::Node(int number) {
    num = number;
}

void Node::addEdge(int dir, int weight) {
   edges.push_back(Edge(dir, weight));
}

int Node::returnEdgeWeight(int dst) {
    return edges[dst].weight;
}