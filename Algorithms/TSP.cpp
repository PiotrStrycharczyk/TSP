//
// Created by Piotr on 07.10.2024.
//
#include "TSP.h"

#include <iostream>
#include <algorithm>
#include <random>
//#include <bits/random.h>

TSP::TSP(Timer& timer, int shortest_path_from_file) : timer_(timer), shortest_path_from_file(shortest_path_from_file) {
    // Inicjalizujemy timer, który jest przekazywany do klasy
    shortestpath = 0;
}
int TSP::getShortestPath() {
    return shortestpath;
}

std::vector<int> TSP::getSolvedPath() {
    return solvedpath;
}

int TSP::bruteForce(std::vector<Node> graph, int V) {

    int startvertex = 0; // Punkt startowy algorytmu
    std::vector<int> vertex; // Wektor wierzchołków do permutacji (bez 0)
    solvedpath.clear(); // Czyszczenie ścieżki rozwiązania

    // Dodajemy wszystkie wierzchołki poza wierzchołkiem startowym do listy do permutacji
    for (int i = 1; i < V; i++) {
        vertex.push_back(i);
    }

    shortestpath = INT_MAX; // Inicjalizacja najkrótszej ścieżki na maksymalną wartość
    do {

        double elapsed_time = timer_.getCounter();
        if (elapsed_time > timer_.time_limit) {
            // Przekroczono dopuszczalny czas
            if_ended_by_iterations = false;
            return shortestpath;
        }

        int current_pathweight = 0; // Waga bieżącej ścieżki
        int k = startvertex; // Zaczynamy od wierzchołka startowego

        // Tworzymy tymczasowy wektor, aby przechowywać bieżącą ścieżkę
        std::vector<int> temp_path;
        temp_path.push_back(k); // Dodajemy wierzchołek startowy

        bool validPath = true; // Flaga sprawdzająca poprawność ścieżki

        // Obliczamy koszt bieżącej ścieżki
        for (int i = 0; i < vertex.size(); i++) {
            int next_vertex = vertex[i];
            int edge_weight = graph[k].returnEdgeWeight(next_vertex);

            // Sprawdzamy, czy istnieje krawędź
            if (edge_weight == -1) {
                validPath = false; // Jeśli nie ma krawędzi, przerywamy tę ścieżkę
                break;
            }

            current_pathweight += edge_weight;
            k = next_vertex;
            temp_path.push_back(k); // Dodajemy kolejny wierzchołek do ścieżki
        }

        // Dodajemy koszt powrotu do miasta startowego
        int return_edge_weight = graph[k].returnEdgeWeight(startvertex);

        if (validPath && return_edge_weight != -1) {
            current_pathweight += return_edge_weight;
            temp_path.push_back(startvertex); // Powrót do wierzchołka startowego

            // Jeżeli bieżący koszt jest mniejszy niż dotychczasowy minimalny, aktualizujemy ścieżkę
            if (current_pathweight < shortestpath) {
                shortestpath = current_pathweight;  // Aktualizujemy minimalny koszt
                solvedpath = temp_path;           // Aktualizujemy ścieżkę, która ma najniższy koszt
            }
        }

    } while (std::next_permutation(vertex.begin(), vertex.end())); // Generujemy wszystkie permutacje

    // Jeśli shortestpath nie zostało zaktualizowane, oznacza to, że nie znaleziono poprawnej ścieżki
    if (shortestpath == INT_MAX) {
        return -1; // Brak rozwiązania
    }


    return shortestpath;
}



bool TSP::ifAllVisited(std::vector<bool> odwiedzone) {//metoda pomocniczna do NN
    for(int i = 0 ; i < odwiedzone.size() ; i++) {
        if (odwiedzone[i] == false) {
            return false;
        }
    }
    return true;

}

int TSP::nearestNeighbour(std::vector<Node> graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost) {
    if (odwiedzone[start] == false) {//ustawienie nextVertex przeslanego w rekrencyjnym wywolaniu na odwiedzony
        odwiedzone[start] = true;
        current_path.push_back(start);  // Add the current node to the path
    }

    bool czyOdwiedzoneWszystkie = ifAllVisited(odwiedzone);

    if (czyOdwiedzoneWszystkie) {//jesli cala droga juz jest znaleziona wystarczy juz tylko polaczyc z wierzcholkiem startowym
        // Check the weight of returning to the start node
        int last_weight = graph[start].returnEdgeWeight(current_path[0]);
        if (last_weight == -1) {
            return -1;  // zla sciezka, bo nie moze wrocic do poczatkowego wierzxholka
        }

        int total_cost = current_cost + last_weight;
        current_path.push_back(current_path[0]);

        // Update the best path if a better solution is found
        if (total_cost < best_cost) {
            best_cost = total_cost;
            best_path = current_path;  // Save the current path as the best
        }

        return best_cost;
    }

    // List of potential candidates with the minimum weight
    std::vector<int> candidates;
    int minWeight = INT_MAX;

    // Select the candidates with the minimum weight
    for (const auto& edge : graph[start].edges) {//szukanie potencjalnych kandydatow (krawedzi o tym samym koszcie)
        if (!odwiedzone[edge.destination] && edge.weight != -1) {
            if (edge.weight < minWeight) {//jesli mniejsza niz poprzednie
                minWeight = edge.weight;
                candidates.clear();
                candidates.push_back(edge.destination);
            } else if (edge.weight == minWeight) {//jesli takie same krawedzie
                candidates.push_back(edge.destination);
            }
        }
    }

    // eksploracja wszystkich rozglezien
    for (int nextVertex : candidates) {
        odwiedzone[nextVertex] = true;
        current_path.push_back(nextVertex);  // Add the vertex to the current path

        // current_cost + minWeight to nowy current cost w kolejnym wowlaniu
        nearestNeighbour(graph, V, nextVertex, odwiedzone, current_cost + minWeight, current_path, best_path, best_cost);

        // Backtracking i przygotowanie przed zwiedzeniem koljengo rozgalezienia
        odwiedzone[nextVertex] = false;
        current_path.pop_back();  // Remove the last vertex from the current path
    }

    return best_cost;
}


int TSP::repetetiveNearestNeighbour(std::vector<Node> graph, int V) {//jedynie ta metoda zmienia pole klasy
    std::vector<bool> odwiedzone(V, false);       // Na początku wszystkie wierzchołki nieodwiedzone
    int current_cost = 0;                         // Początkowy koszt trasy to 0
    std::vector<int> current_path;                // Początkowa trasa jest pusta
    std::vector<int> best_path;                   // Najlepsza trasa, którą chcemy znaleźć
    int best_cost = INT_MAX;                      // Początkowy najlepszy koszt ustawiony na nieskończonoś
    //powyzej dane potrzebne do odpalenia rekurecyjnej metody nearestneighbour

    int best_weight = -1;//dajemy wartosc ujemna zeby przy pierwszej iteracji zostala ta zmienna nadpisana
    std::vector<int> best_way;
    for(int i = 0 ; i < V ; i++) {
        double elapsed_time = timer_.getCounter();
        if (elapsed_time > timer_.time_limit) {
            //przekroczono dopuszczalny czas
            if_ended_by_iterations = false;
            break;
        }

        int result = nearestNeighbour(graph, V, i, odwiedzone, current_cost, current_path, best_path, best_cost);

        //przygotowanie przed kolejna rundą
        odwiedzone.clear();
        odwiedzone.assign(V, false);
        current_cost = 0;
        current_path.clear();

        //i to wierzcholek startowy dla metody nearestneighbour
        if(result != -1 && (best_weight == -1 || best_weight > result)) {
            best_weight = best_cost;
            best_way = best_path;
        }

        if(result == shortest_path_from_file) {
            //if_ended_by_iterations = false;//konczymy bo znalezlismy najlepsze rozwiazanie
            shortestpath = result;//dopiero po calym repetetive nearestneighbour nadpisujemy wartosc pol klasy
            solvedpath = best_way;
            return shortestpath;
        }
    }
    allPathWeights.push_back(best_weight);//tylko dajemy najlepsza
    shortestpath = best_weight;//dopiero po calym repetetive nearestneighbour nadpisujemy wartosc pol klasy
    solvedpath = best_way;
    return shortestpath;
}


int TSP::randomMetod(std::vector<Node> graph, int V) {
    std::random_device rd;  // rd jest wykorzystywane do generowania losowej wartości (ziarno)
    std::mt19937 g(rd());   // tworze generator liczb pseudolosowych, inicjalizowany wartością z urządzenia losowego

    shortestpath = INT_MAX;
    std::vector<int> tab_nieodwiedzonych(V);

    for (int i = 0; i < V; i++) {  // indeksów tyle, ile wierzchołków
        tab_nieodwiedzonych[i] = i;
    }

    while (true) {  // Pętla działa do czasu znalezienia rozwiązania równego rozwiązaniu z pliku lub przekroczenia limitu czasu
        double elapsed_time = timer_.getCounter();  // sprawdzamy, czy nie należy przerwać z powodu przekroczenia czasu
        if (elapsed_time > timer_.time_limit) {
            // przekroczono dopuszczalny czas
            if_ended_by_iterations = false;
            return shortestpath;  // zwrot shortestpath
        }

        std::vector<int> currentPath;  // Ścieżka dla bieżącej iteracji
        std::vector<int> temp_tab_nieodwiedzonych = tab_nieodwiedzonych;  // Tymczasowa lista wierzchołków do odwiedzenia
        int score = 0;

        for (int j = 0; j < V; j++) {
            std::uniform_int_distribution<> dist(0, temp_tab_nieodwiedzonych.size() - 1);  // losowanie indeksu
            int randIndex = dist(g);  // generowanie liczby z distribution liczb z zadanego wyżej przedziału

            int currentNode = temp_tab_nieodwiedzonych[randIndex];
            currentPath.push_back(currentNode);  // dodanie wierzchołka do ścieżki

            if (j > 0) {
                // Obliczamy wagę krawędzi między bieżącym a poprzednim wierzchołkiem
                int prevNode = currentPath[j - 1];
                int weight = graph[prevNode].returnEdgeWeight(currentNode);

                // Sprawdzamy, czy istnieje połączenie
                if (weight == -1) {
                    // Brak połączenia - przechodzimy do następnej iteracji
                    score = INT_MAX;
                    break;
                }
                score += weight;
            }

            // Usuwamy odwiedzony wierzchołek z listy
            temp_tab_nieodwiedzonych.erase(temp_tab_nieodwiedzonych.begin() + randIndex);
        }

        if (score != INT_MAX) {
            // Łączymy ostatni wierzchołek z pierwszym, aby zamknąć cykl
            int lastNode = currentPath.back();
            int firstNode = currentPath.front();
            int lastEdgeWeight = graph[lastNode].returnEdgeWeight(firstNode);

            if (lastEdgeWeight != -1) {  // Jeśli połączenie istnieje
                score += lastEdgeWeight;
                currentPath.push_back(firstNode);  // Zamyka cykl

                if (score < shortestpath) {
                    shortestpath = score;
                    solvedpath = currentPath;

                    // Jeśli znaleziono rozwiązanie równe temu z pliku, kończymy algorytm
                    if (shortestpath == shortest_path_from_file) {
                        //if_ended_by_iterations = false;
                        return shortestpath;
                    }
                }
            }
        }
    }
}


bool TSP::IfEndedWithIterations() {
    return if_ended_by_iterations;
}



