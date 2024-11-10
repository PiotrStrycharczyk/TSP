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

int BranchNBound::lowestChecker(std::vector<Node> graph, int i, int V) {

}

int BranchNBound::firstMin(std::vector<Node> graph, int i, int V,std::vector<bool>& visited) {//zwraca minimalna wage krawedzi wychodzaca z wierzch i
    int min = INT_MAX;
    for (int k = 0; k < V; k++)
        if (graph[i].returnEdgeWeight(k) < min && i != k)
            if (!visited[i])
                min = graph[i].returnEdgeWeight(k);
    return min;
}

int BranchNBound::secondMin(std::vector<Node> graph, int i, int V) {//zwraca druga najmniejsza wage krawedzi wychodzacej z wierzch i
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < V; j++) {
        if (i == j)
            continue;

        if (graph[i].returnEdgeWeight(j) <= first) {//i nr wiersza j nr kolumny(jesli krawedz jest mniejsza od najmniejszej to
            second = first;
            first = graph[i].returnEdgeWeight(j);
        } else if (graph[i].returnEdgeWeight(j) <= second &&//jesli krawedz znaleziona jest wieksza od first ale mniejsza lub rowna sec
                   graph[i].returnEdgeWeight(j) != first)
            second = graph[i].returnEdgeWeight(j);
    }
    return second;
}

void BranchNBound::TSPRecurrent(std::vector<Node> graph, int curr_bound, int curr_weight,
                           int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited) {
    if (level == V) {
        if (graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]) != -1) {
            int curr_res = curr_weight + graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]);

            if (curr_res < final_res) {
                copyToFinal(curr_path.data(), V); // `data()` daje wskaźnik do wektora
                final_res = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < V; i++) {
        if (graph[curr_path[level - 1]].returnEdgeWeight(i) != -1 && !visited[i]) {
            int temp = curr_bound;
            curr_weight += graph[curr_path[level - 1]].returnEdgeWeight(i);//do obecnej wagi sciezki dodajemy krawedz i-tą wychodzaca z nodea na którym bylismy w poprzedniej iter

            if (level == 1) {
                //curr_bound -= ((firstMin(graph, curr_path[level - 1], V) + firstMin(graph, i, V)) / 2);
                curr_bound -= firstMin(graph, i, V,visited);
            }
            else {
                // curr_bound -= ((secondMin(graph, curr_path[level - 1], V) + firstMin(graph, i, V)) / 2);
                curr_bound -= firstMin(graph, i, V,visited);
            }

            if (curr_bound + curr_weight < final_res) {//jesli curr_bound i waga obcenej sciezki jest >=final result to konczymy badanie tej galezi
                curr_path[level] = i;
                visited[i] = true;

                // Wywołanie rekurencyjne TSPRec na kolejnym poziomie
                TSPRecurrent(graph, curr_bound, curr_weight, level + 1, curr_path, V, visited);
            }

            // Cofanie się (backtracking)
            curr_weight -= graph[curr_path[level - 1]].returnEdgeWeight(i);
            curr_bound = temp;

            std::fill(visited.begin(), visited.end(), false); // Zamiast memset
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

// void BranchNBound::TSPRecurrent(std::vector<Node> graph, int curr_bound, int curr_weight,
//                            int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited) {
//     if (level == V) {
//         // Jeśli osiągnięto poziom końcowy, sprawdź połączenie do wierzchołka początkowego
//         if (graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]) != -1) {
//             int curr_res = curr_weight + graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]);
//
//             if (curr_res < final_res) {
//                 copyToFinal(curr_path.data(), V);
//                 final_res = curr_res;
//             }
//         }
//         return;
//     }
//
//     for (int i = 0; i < V; i++) {
//         if (graph[curr_path[level - 1]].returnEdgeWeight(i) != -1 && !visited[i]) {
//             int temp_weight = curr_weight + graph[curr_path[level - 1]].returnEdgeWeight(i);
//
//             // Sprawdź, czy nowa estymacja (curr_weight + curr_bound) jest lepsza od aktualnego wyniku
//             if (temp_weight < final_res) {
//                 curr_path[level] = i;
//                 visited[i] = true;
//
//                 // Rekurencyjne wywołanie dla kolejnego poziomu
//                 TSPRecurrent(graph, curr_bound, temp_weight, level + 1, curr_path, V, visited);
//             }
//
//             // Backtracking: przywrócenie poprzednich stanów
//             visited[i] = false;
//         }
//     }
// }

void BranchNBound::TSPDFS(std::vector<Node> graph, int V) {//TSP DFS
    final_res = INT_MAX;
    final_path.clear();
    std::vector<int> curr_path(V + 1, -1); // Zamiast int curr_path[V+1];
    std::vector<bool> visited(V, false);   // Zamiast tablicy visited
    final_path.resize(V+1);//rezerwujemy dla calej sciezki na drogę

    TSP nearestNeighbour;
    int curr_bound = nearestNeighbour.nearestNeighbour(graph, V, 0);//startujemy z wierzcholka 0
    std::cout<<"NN wynik: "<<curr_bound<<std::endl;

    visited[0] = true;
    curr_path[0] = 0;
    // Wywołanie rekurencyjne z użyciem wektorów
    TSPRecurrent(graph, curr_bound, 0, 1, curr_path, V, visited);
}


std::vector<int> BranchNBound::returnRoute() {
    return final_path;
}

void BranchNBound::TSPBFS(std::vector<Node> graph, int V) {

}
