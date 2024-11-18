//
// Created by Piotr on 18.11.2024.
//

#ifndef GENERATEGRAPH_H
#define GENERATEGRAPH_H
#include "../Graph/Node.h"
#include <vector>


class GenerateGraph {
public:
    void generateAsymetric(std::vector<Node>&, int);
    void generateSymetric(std::vector<Node>&, int);//zwracaja wektor ktory jest naszym grafem

};



#endif //GENERATEGRAPH_H
