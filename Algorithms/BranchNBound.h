//
// Created by Piotr on 03.11.2024.
//

#ifndef BRANCHNBOUND_H
#define BRANCHNBOUND_H
#include <vector>
#include "../Graph/Node.h"


class BranchNBound {
public:
    BranchNBound();
    void TSPDFS(std::vector<Node>, int V);//DFS
    void TSPBFS(std::vector<Node>graph, int V);//BFS
    std::vector<int> returnRoute();
    int getFinalRes();

private:
    std::vector<int> final_path;
    int final_res;
    void copyToFinal(int curr_path[], int V);
    void TSPRecurrent(std::vector<Node> graph, int& curr_bound, int curr_weight, int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited);
    int lowestChecker(std::vector<Node> graph, int i, int V);
};



#endif //BRANCHNBOUND_H
