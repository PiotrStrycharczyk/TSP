#include "BranchNBound.h"
#include <climits>
#include <vector>
#include <algorithm> // Dla std::fill
#include <iostream>
#include <queue>

#include "TSP.h"

BranchNBound::BranchNBound() {

}

void BranchNBound::copyToFinal(int curr_path[], int V) {//kopiowanie sciezki do zmiennej, ktora bd wynikiem algorytmu
    for (int i = 0; i < V; i++)
        final_path[i] = curr_path[i];
    final_path[V] = curr_path[0];
}

void BranchNBound::TSPRecurrent(std::vector<Node> graph, int& upper_bound, int curr_weight,
                           int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited) {
    if (level == V) {
        if (graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]) != -1) {//powrót do początkowego wierzcholka
            int curr_res = curr_weight + graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]);

            if (curr_res < upper_bound) {
                copyToFinal(curr_path.data(), V); // `data()` daje wskaźnik do wektora
                final_res = curr_res;//przypisanie do zmiennej final rest_najlepszego obecnie rozwiazania
                upper_bound = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < V; i++) {
        if (graph[curr_path[level - 1]].returnEdgeWeight(i) != -1 && !visited[i]) {

            curr_weight += graph[curr_path[level - 1]].returnEdgeWeight(i);//do obecnej wagi sciezki dodajemy krawedz i-tą wychodzaca z nodea na którym bylismy w poprzedniej iter

            if (curr_weight < upper_bound) {//jesli dotychczasowa waga sciezki jest ciągle mniejsza od upperbound to kontynuujemy schodzenie
                curr_path[level] = i;
                visited[i] = true;

                // Wywołanie rekurencyjne TSPRec na kolejnym poziomie
                TSPRecurrent(graph, upper_bound, curr_weight, level + 1, curr_path, V, visited);
            }

            // Cofanie się (backtracking)
            curr_weight -= graph[curr_path[level - 1]].returnEdgeWeight(i);

            std::fill(visited.begin(), visited.end(), false);//przypisuje wartosc w zakresie[begin;end)
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = true;
        }
    }
}


void BranchNBound::TSPDFS(std::vector<Node> graph, int V) {//TSP DFS - przeszukiwanie po glebokosci
    final_res = INT_MAX;
    final_path.clear();
    std::vector<int> curr_path(V + 1, -1); // Zamiast int curr_path[V+1];
    std::vector<bool> visited(V, false);   // Zamiast tablicy visited
    final_path.resize(V+1);//rezerwujemy dla calej sciezki na drogę

    int upper_bound = INT_MAX;

    visited[0] = true;//zaczcynamy od zerowego
    curr_path[0]= 0;

    TSPRecurrent(graph, upper_bound, 0, 1, curr_path, V, visited);
}


std::vector<int> BranchNBound::returnRoute() {
    return final_path;
}

void BranchNBound::TSPBFS(std::vector<Node> graph, int V) {

}

int BranchNBound::getFinalRes() {
    return final_res;
}

