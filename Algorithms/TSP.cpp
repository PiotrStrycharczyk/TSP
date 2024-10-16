//
// Created by Piotr on 07.10.2024.
//
#include "TSP.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <bits/random.h>

TSP::TSP() {
    shortestpath = 0;//podczas tworzenia obiektu wartosc jest nadawana na 0
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
        int current_pathweight = 0; // waga bieżącej ścieżki
        int k = startvertex; // zaczynamy od wierzchołka startowego

        // Tworzymy tymczasowy wektor, aby przechowywać bieżącą ścieżkę
        std::vector<int> temp_path;
        temp_path.push_back(k); // Dodajemy wierzchołek startowy

        bool validPath = true; // flaga do sprawdzenia, czy ścieżka jest poprawna

        // Obliczamy koszt bieżącej ścieżki
        for (int i = 0; i < vertex.size(); i++) {
            int next_vertex = vertex[i];
            int edge_weight = graph[k].edges[next_vertex].weight;

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
        int return_edge_weight = graph[k].edges[startvertex].weight;

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

int TSP::nearestNeighbour(std::vector<Node> graph, int V, int start) {
    shortestpath = 0;
    solvedpath.clear();//reset zmiennych przed algorytmem

    std::vector<bool> odwiedzone(V, false);//wartosc poczatkowa jako false

    int startvertex = start;
    int nowvertex = startvertex;//losujemy wierzcholek poczatkowy

    solvedpath.push_back(startvertex);//startujemy od wylosowanego
    odwiedzone[startvertex] = true;
    //novertex obecny wierzch na ktorym jestesmy
    //nextvertex to kolejny ktory mamy odwiedzic
    for (int i = 1; i < V; ++i) {
        int nextVertex = -1;
        int minWeight = INT_MAX; // Ustawiamy maksymalną wartość

        // Szukamy najbliższego nieodwiedzonego wierzchołka
        for (const auto& edge : graph[nowvertex].edges) {
            if (!odwiedzone[edge.destination] && edge.weight < minWeight && edge.weight != -1) {
                minWeight = edge.weight;
                nextVertex = edge.destination;
            }
        }

        // Przechodzimy do najbliższego wierzchołka
        if (nextVertex == -1) {
            return -1;//koniec algorytmu brak jakiejkolwiek wychodzjacej z wierzcholka krawedzi
        }
        solvedpath.push_back(nextVertex);
        shortestpath += minWeight;
        odwiedzone[nextVertex] = true;
        nowvertex = nextVertex;
    }
    int returnWeight = graph[nowvertex].edges[startvertex].weight;
    if (returnWeight != -1) {
        solvedpath.push_back(startvertex); // Powrót do wierzchołka początkowego
        shortestpath += returnWeight;
    } else {
        shortestpath = -1; // Brak trasy powrotnej
        solvedpath.clear();
        return -1;
    }

    return shortestpath;
}

int TSP::repetetiveNearestNeighbour(std::vector<Node> graph, int V) {
    int best_weight = -1;//dajemy wartosc ujemna zeby przy pierwszej iteracji zostala ta zmienna nadpisana
    std::vector<int> best_way;
    for(int i = 0 ; i < V ; i++) {
        int result = nearestNeighbour(graph, V, i);//i to wierzcholek startowy dla metody nearestneighbour
        if(result != -1 && (best_weight == -1 || best_weight > result)) {
            best_weight = shortestpath;
            best_way = solvedpath;
        }
    }
    shortestpath = best_weight;
    solvedpath = best_way;
    return shortestpath;
}


int TSP::randomMetod(std::vector<Node> graph, int V) {//jesli algorytm znajdzie ze miedzy dwoma V nie ma krawedzi, to zmienne klasowe sa nie zmieniane
    std::random_device rd;// rd jest wykorzystywane do generowania losowej wartosci
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
        std::uniform_int_distribution<> dist(0, tab_nieodwiedzonych.size() - 1);//losowanie indexu z przedzialu [ ; ]
        int randIndex = dist(g);//generowanie liczby z distribution liczb z zadanego wyzej przedzialu

        int currentNode = tab_nieodwiedzonych[randIndex];
        selectedPath.push_back(currentNode);//dodanie pocz wierzcholka do odwiedzonych

        if (j > 0) {
            // Obliczamy wagę krawędzi między bieżącym a poprzednim wierzchołkiem
            int prevNode = selectedPath[j - 1];
            int weight = graph[prevNode].edges[currentNode].weight;

            // Sprawdzamy, czy istnieje połączenie
            if (weight == -1) {
                return -1;  // Brak połączenia - odrzucamy tę permutację
            }
            score += weight;
        }
        tab_nieodwiedzonych.erase(tab_nieodwiedzonych.begin() + randIndex);  // Usuwamy wierzchołek z listy (po usunieciu jest o jeden mniej elementow) w vectorze

    }

    // Łączymy ostatni wierzchołek z pierwszym, aby zamknąć cykl
    int lastNode = selectedPath.back();
    int firstNode = selectedPath.front();
    int lastEdgeWeight = graph[lastNode].edges[firstNode].weight;

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

void TSP::randomMetodIterations(std::vector<Node> graph, int V, int iter_rand) {
    TSP tsp;//tworzenie instacji klasy tsp

    int best_weight = INT_MAX;
    int current_weight;
    std::vector<int> best_path;
    std::vector<int> current_path;
    for(int i = 0 ; i < iter_rand ; i++) {
        int result = tsp.randomMetod(graph, V);
        if(result != -1) {
            current_weight = tsp.getShortestPath();
            current_path = tsp.getSolvedPath();
            if(current_weight < best_weight) {
                best_weight = current_weight;
                best_path = current_path;
            }
        }
    }
    shortestpath = best_weight;
    solvedpath = best_path;
}



