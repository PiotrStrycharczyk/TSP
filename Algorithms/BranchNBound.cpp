#include "BranchNBound.h"
#include <climits>
#include <vector>
#include <algorithm> // Dla std::fill
#include <iostream>
#include <queue>

#include "TSP.h"

// w BranchNBound.cpp
int BranchNBound::nn_result = INT_MAX; // jesli nn sie nie powiedzie

BranchNBound::BranchNBound(Timer& timer) : timer_(timer){

}

void BranchNBound::copyToFinal(int curr_path[], int V) {//kopiowanie sciezki do zmiennej, ktora bd wynikiem algorytmu
    for (int i = 0; i < V; i++)
        final_path[i] = curr_path[i];
    final_path[V] = curr_path[0];
}

void BranchNBound::TSPRecurrent(std::vector<Node> graph, int& upper_bound, int curr_weight,
                           int level, std::vector<int>& curr_path, int V, std::vector<bool>& visited) {
    // Sprawdzanie warunku zakończenia
    double elapsed_time = timer_.getCounter();
    if (elapsed_time > timer_.time_limit) {
        // Zatrzymujemy dalsze wykonywanie
        if_ended_by_iterations = false; // Ustawiamy flagę informującą o przekroczeniu czasu
        return;
    }

    if (level == V) {
        if (graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]) != -1) { // Powrót do początkowego wierzchołka
            int curr_res = curr_weight + graph[curr_path[level - 1]].returnEdgeWeight(curr_path[0]);

            if (curr_res <= upper_bound) {
                copyToFinal(curr_path.data(), V); // `data()` daje wskaźnik do wektora
                final_res = curr_res; // Przypisanie najlepszego obecnie rozwiązania
                upper_bound = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < V; i++) {
        // Dodajemy warunek sprawdzający zakończenie rekurencji
        if (!if_ended_by_iterations) {//jesli chcemy zatrzymac kolejne wywolania z powodu braku czasu to wchodzimy do tego if-a
            return; // Zatrzymujemy pętlę, jeśli czas już się skończył
        }

        if (graph[curr_path[level - 1]].returnEdgeWeight(i) != -1 && !visited[i]) {
            curr_weight += graph[curr_path[level - 1]].returnEdgeWeight(i); // Aktualizujemy wagę ścieżki

            if (curr_weight < upper_bound) { // Jeśli dotychczasowa waga jest mniejsza od górnej granicy
                curr_path[level] = i;
                visited[i] = true;

                // Wywołanie rekurencyjne
                TSPRecurrent(graph, upper_bound, curr_weight, level + 1, curr_path, V, visited);
            }

            // Cofanie się (backtracking)
            curr_weight -= graph[curr_path[level - 1]].returnEdgeWeight(i);
            std::fill(visited.begin(), visited.end(), false); // Resetowanie odwiedzonych wierzchołków
            for (int j = 0; j <= level - 1; j++) {
                visited[curr_path[j]] = true;
            }
        }
    }
}



void BranchNBound::TSPDFS(std::vector<Node> graph, int V) {//TSP DFS - przeszukiwanie po glebokosci
    final_res = INT_MAX;
    final_path.clear();
    std::vector<int> curr_path(V + 1, -1); // Zamiast int curr_path[V+1];
    std::vector<bool> visited(V, false);   // Zamiast tablicy visited
    final_path.resize(V+1);//rezerwujemy dla calej sciezki na drogę

    int upper_bound = nn_result;

    visited[0] = true;//zaczcynamy od zerowego
    curr_path[0]= 0;

    TSPRecurrent(graph, upper_bound, 0, 1, curr_path, V, visited);
}

void BranchNBound::TSPBFS(std::vector<Node> graph, int V) {
    int upper_bound = nn_result; // Najlepszy znany koszt (rozwiązanie optymalne)
    std::vector<int> best_path; // Najlepsza znaleziona ścieżka

    // Inicjalizacja pierwszego stanu
    State initialState;
    initialState.level = 0;
    initialState.currentCity = 0;
    initialState.costSoFar = 0;
    initialState.path.push_back(0);

    // Tworzymy kolejkę BFS
    std::queue<State> q;
    q.push(initialState);

    while (!q.empty()) {
        double elapsed_time = timer_.getCounter();
        if (elapsed_time > timer_.time_limit) {
            // Zatrzymujemy dalsze wykonywanie
            if_ended_by_iterations = false; // Ustawiamy flagę informującą o przekroczeniu czasu
            final_res = upper_bound;
            final_path = best_path;//przypisanie dotychczasowego osiagnietego najlepszego rozwiazania
            return;
        }
        // Pobierz aktualny stan z kolejki
        State current = q.front();
        q.pop();

        // Jeśli osiągnęliśmy ostatni poziom, sprawdzamy powrót do miasta startowego
        if (current.level == V - 1) {
            int returnCost = graph[current.currentCity].returnEdgeWeight(0); // Koszt powrotu do startu
            if (returnCost != -1) { // Jeśli istnieje połączenie
                int totalCost = current.costSoFar + returnCost;

                // Jeśli znalezione rozwiązanie jest lepsze, zapamiętujemy je
                if (totalCost <= upper_bound) {
                    upper_bound = totalCost;
                    best_path = current.path;
                    best_path.push_back(0); // Dodajemy powrót do miasta startowego
                }
            }
            continue;
        }

        // Generuj kolejne stany
        for (int i = 0; i < V; i++) {
            // Jeśli miasto `i` nie zostało odwiedzone i istnieje połączenie
            int edgeCost = graph[current.currentCity].returnEdgeWeight(i);
            if (!checkIfVisited(current.path, i) && edgeCost != -1) {

                // Dodajemy stan do kolejki tylko jeśli koszt jest obiecujący
                if (edgeCost + current.costSoFar < upper_bound) {//sprawdzamy wczesniej aby niepotrzebne nie tworzyc obiektu struktury
                    State nextState = current;
                    nextState.level++;
                    nextState.currentCity = i;
                    nextState.costSoFar += edgeCost;
                    nextState.path.push_back(i);
                    q.push(nextState);
                }
            }
        }
    }

    // Zapisz najlepsze rozwiązanie
    final_res = upper_bound;
    final_path = best_path;
}

bool BranchNBound::checkIfVisited(std::vector<int> path, int k) {
    for(int i = 0 ; i < path.size(); i++) {
        if (path[i] == k)
            return true;
    }
    return false;

}

void BranchNBound::LowestCost(std::vector<Node> graph, int V) {
    int upper_bound = nn_result; // Najlepszy znany koszt (rozwiązanie optymalne)
    std::vector<int> best_path; // Najlepsza znaleziona ścieżka

    // Inicjalizacja pierwszego stanu
    StateLowestCost initialState;
    initialState.level = 0;
    initialState.currentCity = 0;
    initialState.costSoFar = 0;
    initialState.path.push_back(0);

    // Tworzymy kolejkę priorytetową
    std::priority_queue<StateLowestCost> pq;
    pq.push(initialState);

    while (!pq.empty()) {
        double elapsed_time = timer_.getCounter();
        if (elapsed_time > timer_.time_limit) {
            // Zatrzymujemy dalsze wykonywanie
            if_ended_by_iterations = false; // Ustawiamy flagę informującą o przekroczeniu czasu
            final_res = upper_bound;
            final_path = best_path; // Przypisanie dotychczasowego najlepszego rozwiązania
            return;
        }

        // Pobierz aktualny stan z kolejki priorytetowej
        StateLowestCost current = pq.top();
        pq.pop();

        // Jeśli osiągnęliśmy ostatni poziom, sprawdzamy powrót do miasta startowego
        if (current.level == V - 1) {
            int returnCost = graph[current.currentCity].returnEdgeWeight(0); // Koszt powrotu do startu
            if (returnCost != -1) { // Jeśli istnieje połączenie
                int totalCost = current.costSoFar + returnCost;

                // Jeśli znalezione rozwiązanie jest lepsze, zapamiętujemy je
                if (totalCost <= upper_bound) {
                    upper_bound = totalCost;
                    best_path = current.path;
                    best_path.push_back(0); // Dodajemy powrót do miasta startowego
                }
            }
            continue;
        }

        // Generuj kolejne stany
        for (int i = 0; i < V; i++) {
            // Jeśli miasto `i` nie zostało odwiedzone i istnieje połączenie
            int edgeCost = graph[current.currentCity].returnEdgeWeight(i);
            if (!checkIfVisited(current.path, i) && edgeCost != -1) {
                // Dodajemy stan do kolejki tylko jeśli koszt jest obiecujący
                if (edgeCost + current.costSoFar < upper_bound) { // Sprawdzamy wcześniej, aby niepotrzebnie nie tworzyć obiektów
                    StateLowestCost nextState = current;
                    nextState.level++;
                    nextState.currentCity = i;
                    nextState.costSoFar += edgeCost;
                    nextState.path.push_back(i);
                    pq.push(nextState); // Dodajemy stan do kolejki priorytetowej
                }
            }
        }
    }

    // Zapisz najlepsze rozwiązanie
    final_res = upper_bound;
    final_path = best_path;
}




int BranchNBound::getShortestPath() {
    return final_res;
}

std::vector<int> BranchNBound::getSolvedPath() {
    return final_path;
}
bool BranchNBound::IfEndedWithIterations() {
    return if_ended_by_iterations;
}


