//
// Created by Piotr on 18.11.2024.
//
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "GenerateGraph.h"

void GenerateGraph::generateAsymetric(std::vector<Node>& graph, int V) {
    // Czyszczenie grafu
    if (!graph.empty()) {
        graph.clear();
    }

    // Inicjalizacja wierzchołków
    for (int i = 0; i < V; i++) {
        graph.push_back(Node(i)); // Numerujemy wierzchołki od 0 do V-1
    }

    // Generowanie losowych krawędzi
    std::srand(std::time(0)); // Inicjalizacja generatora liczb pseudolosowych
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j) { // Brak krawędzi do samego siebie
                int weight = 10 + std::rand() % 90; // Losowanie wagi w zakresie 10-99
                graph[i].addEdge(j, weight);
            } else
                graph[i].addEdge(j,-1);//przekatna
        }
    }
}

// Funkcja generująca graf symetryczny (nieskierowany)
void GenerateGraph::generateSymetric(std::vector<Node>& graph, int V) {
    // Czyszczenie grafu
    if (!graph.empty()) {
        graph.clear();
    }

    // Inicjalizacja wierzchołków
    for (int i = 0; i < V; i++) {
        graph.push_back(Node(i)); // Numerujemy wierzchołki od 0 do V-1
    }

    // Generowanie losowych krawędzi
    std::srand(std::time(0)); // Inicjalizacja generatora liczb pseudolosowych
    for (int i = 0; i < V; i++) {
        for (int j = i; j < V; j++) {
            if (i == j) {
                graph[i].addEdge(j, -1); // Wartość na przekątnej
            } else {
                int weight = 10 + std::rand() % 90; // Losowanie wagi w zakresie 10-99
                graph[i].addEdge(j, weight);
                graph[j].addEdge(i, weight); // Dodajemy również w drugą stronę
            }
        }
    }
}



