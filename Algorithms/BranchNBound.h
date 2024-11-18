//
// Created by Piotr on 03.11.2024.
//

#ifndef BRANCHNBOUND_H
#define BRANCHNBOUND_H
#include <vector>
#include "../Graph/Node.h"
#include "../Counting/Timer.h"

class BranchNBound {
public:
    BranchNBound(Timer& timer);
    void TSPDFS(std::vector<Node>, int V);//DFS
    void TSPBFS(std::vector<Node>graph, int V);//BFS
    void LowestCost(std::vector<Node>graph, int V);
    std::vector<int> getSolvedPath();
    int getShortestPath();

private:
    Timer& timer_;
    bool if_ended_by_iterations = true;//zakldamy ze konczy sie przez iteracje ale gdy jednak przez czas zwraca false

    std::vector<int> final_path;
    int final_res;
    void copyToFinal(int curr_path[], int V);
    void TSPRecurrent(std::vector<Node> graph, int& curr_bound, int curr_weight, int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited);

    struct State {//struktura potrzebna do BFS-a
        int level;
        int currentCity;
        int costSoFar;
        //std::vector<bool> visited;
        std::vector<int>path;
    };
    bool checkIfVisited(std::vector<int> path, int k);


    struct StateLowestCost {
        int level; // Poziom w drzewie przeszukiwań (odległość od korzenia)
        int currentCity; // Bieżące miasto
        int costSoFar; // Dotychczasowy koszt
        std::vector<int> path; // Ścieżka odwiedzonych miast

        // Porównanie dla kolejki priorytetowej (najmniejszy koszt ma najwyższy priorytet)
        bool operator<(const StateLowestCost& other) const {
            return costSoFar > other.costSoFar; // Odwrócone porównanie dla priorytetowej kolejki (min heap)
        }
    };
public:
    static int nn_result;
    bool IfEndedWithIterations();

};



#endif //BRANCHNBOUND_H
