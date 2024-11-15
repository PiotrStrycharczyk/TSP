//
// Created by Piotr on 07.10.2024.
//
#include "TSP.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <bits/random.h>

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

    int startvertex = 0; // punkt startowy algorytmu
    std::vector<int> vertex; // wektor wszystkich poza pierwszym wierzchołków
    solvedpath.push_back(startvertex); // dodanie do rozwiązania wierzchołka startowego

    // Dodajemy wszystkie wierzchołki poza wierzchołkiem startowym do listy do permutacji
    for (int i = 0; i < V; i++)
        if (i != startvertex)
            vertex.push_back(i);

    shortestpath = INT_MAX; // ustawiamy początkową wartość najkrótszej ścieżki na maksymalną wartość

    do {
        double elapsed_time = timer_.getCounter();
        if (elapsed_time > timer_.time_limit) {
            //przekroczono dopuszczalny czas
            if_ended_by_iterations = false;
            return shortestpath;//jesli zwracamy -2 to znaczy że przekroczono juz czas
        }

        int current_pathweight = 0; // waga bieżącej ścieżki
        int k = startvertex; // zaczynamy od wierzchołka startowego

        // Tworzymy tymczasowy wektor, aby przechowywać bieżącą ścieżkę
        std::vector<int> temp_path;
        temp_path.push_back(k); // Dodajemy wierzchołek startowy

        bool validPath = true; // flaga do sprawdzenia, czy ścieżka jest poprawna

        // Obliczamy koszt bieżącej ścieżki
        for (int i = 0; i < vertex.size(); i++) {
            int next_vertex = vertex[i];
            int edge_weight = graph[k].returnEdgeWeight(next_vertex);

            // Sprawdzamy, czy istnieje krawędź
            if (edge_weight == -1) {
                validPath = false; // jeśli nie ma krawędzi, przerywamy tę ścieżkę
                break;
            }

            current_pathweight += edge_weight;
            k = next_vertex;
            temp_path.push_back(k); // Dodajemy kolejne miasto do ścieżki
        }

        // Dodajemy koszt powrotu do miasta startowego, jeśli istnieje krawędź powrotna
        //int return_edge_weight = graph[k].edges[startvertex].weight;
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

    } while (std::next_permutation(vertex.begin(), vertex.end())); // generujemy wszystkie permutacje

    // Jeśli shortestpath nie zostało zaktualizowane, oznacza to, że nie znaleziono poprawnej ścieżki
    if (shortestpath == INT_MAX) {
        return -1; // brak rozwiązania
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


int TSP::repetetiveNearestNeighbour(std::vector<Node> graph, int V, std::vector<bool>& odwiedzone,//ta metoda nie ma start bo sama ja wylicza
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost) {//jedynie ta metoda zmienia pole klasy

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
        ////////////////////////////////
        //i to wierzcholek startowy dla metody nearestneighbour
        if(result != -1 && (best_weight == -1 || best_weight > result)) {
            best_weight = best_cost;
            best_way = best_path;
        }
    }
    shortestpath = best_weight;//dopiero po calym repetetive nearestneighbour nadpisujemy wartosc pol klasy
    solvedpath = best_way;
    return shortestpath;
}


int TSP::randomMetod(std::vector<Node> graph, int V) {//jesli algorytm znajdzie ze miedzy dwoma V nie ma krawedzi, to zmienne klasowe sa nie zmieniane
    std::random_device rd;// rd jest wykorzystywane do generowania losowej wartosci (ziarno)
    std::mt19937 g(rd());//tworze generator liczb pseudolosowych, jest on inicjalizowany wartoscia uzyskana z urzadzenia losowego


    shortestpath = INT_MAX;
    std::vector<int> tab_nieodwiedzonych(V);

    for(int i = 0 ; i < V ; i++) {//indexow tyle, ile wierzcholkow
        tab_nieodwiedzonych[i] = i;
    }

    int score = 0;

    // Wektor do przechowywania losowo wybranych wierzchołków
    std::vector<int> selectedPath;

    for(int j = 0 ; j < V ; j++) {
        double elapsed_time = timer_.getCounter();//sprawdzamy czy nie nalezy przerwac z powodu przekroczenia czasu
        if (elapsed_time > timer_.time_limit) {
            //przekroczono dopuszczalny czas
            if_ended_by_iterations = false;
            break;
        }
        std::uniform_int_distribution<> dist(0, tab_nieodwiedzonych.size() - 1);//losowanie indexu z przedzialu [ ; ] (kazdy element ma takie samo prawdopodobieństwo na trafienie
        int randIndex = dist(g);//generowanie liczby z distribution liczb z zadanego wyzej przedzialu

        int currentNode = tab_nieodwiedzonych[randIndex];
        selectedPath.push_back(currentNode);//dodanie pocz wierzcholka do odwiedzonych

        if (j > 0) {
            // Obliczamy wagę krawędzi między bieżącym a poprzednim wierzchołkiem
            int prevNode = selectedPath[j - 1];
            int weight = graph[prevNode].returnEdgeWeight(currentNode);

            // Sprawdzamy, czy istnieje połączenie
            if (weight == -1) {
                return -1;  // Brak połączenia - odrzucamy
            }
            score += weight;
        }
        tab_nieodwiedzonych.erase(tab_nieodwiedzonych.begin() + randIndex);  // Usuwamy wierzchołek z listy (po usunieciu jest o jeden mniej elementow) w vectorze

    }

    // Łączymy ostatni wierzchołek z pierwszym, aby zamknąć cykl
    int lastNode = selectedPath.back();
    int firstNode = selectedPath.front();
    int lastEdgeWeight = graph[lastNode].returnEdgeWeight(firstNode);

    // Sprawdzamy, czy istnieje połączenie
    if (lastEdgeWeight == -1) {
        return -1;  // Brak połączenia - odrzucamy tę permutację
    }

    score += lastEdgeWeight;
    selectedPath.push_back(firstNode);  // Zamyka cykl

    // Zapisujemy najlepsze rozwiązanie
    if (score < shortestpath) {
        shortestpath = score;
        solvedpath = selectedPath;
    }
    if(shortestpath == INT_MAX) {
        return -1;//nie znaleziono sceizki bo nie poprawiono rozwiazania chociaz raz
    }

    return shortestpath;
}

bool TSP::getInfoHowEnded() {
    return if_ended_by_iterations;
}



