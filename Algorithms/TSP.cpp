//
// Created by Piotr on 07.10.2024.
//
#include <iostream>
#include <algorithm>
#include "TSP.h"

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

    int startvertex = 0;//punkt startowy algorytmu
    std::vector<int> vertex;//wektor wszystkich poza pierwszym wierzcholkow
    solvedpath.push_back(startvertex);//dodanie do rozwiazania wierz pocz

    for (int i = 0; i < V; i++)
        if (i != startvertex)
            vertex.push_back(i);

    shortestpath = INT_MAX;
    do {

        int current_pathweight = 0;
        int k = startvertex;

        // Tworzymy tymczasowy wektor, aby przechowywać bieżącą ścieżkę
        std::vector<int> temp_path;
        temp_path.push_back(k); // Dodajemy wierzchołek startowy

        // Obliczamy koszt bieżącej ścieżki
        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += graph[k].edges[vertex[i]].weight;
            k = vertex[i];
            temp_path.push_back(k); // Dodajemy kolejne miasto do ścieżki
        }

        // Dodajemy koszt powrotu do miasta startowego
        current_pathweight += graph[k].edges[startvertex].weight;
        temp_path.push_back(startvertex); // Powrót do wierzchołka startowego

        // Jeżeli bieżący koszt jest mniejszy niż dotychczasowy minimalny, aktualizujemy ścieżkę
        if (current_pathweight < shortestpath) {
            shortestpath = current_pathweight;  // Aktualizujemy minimalny koszt
            solvedpath = temp_path;           // Aktualizujemy ścieżkę, która ma najniższy koszt
        }


    } while (next_permutation(vertex.begin(), vertex.end()));

    return shortestpath;
}

int TSP::nearestNeighbour(std::vector<Node> graph, int V) {//byc moze trzeba zrobic zeby metoda startowala z kazdego wierzcholka i na koniec brala tylko najlepszy wynik
    std::vector<bool> odwiedzone(V, false);//wartosc poczatkowa jako false

    srand(time(0)); // Inicjalizacja generatora losowego
    int startvertex = rand() % V;
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
            if (!odwiedzone[edge.destination] && edge.weight < minWeight) {
                minWeight = edge.weight;
                nextVertex = edge.destination;
            }
        }

        // Przechodzimy do najbliższego wierzchołka
        if (nextVertex != -1) {
            solvedpath.push_back(nextVertex);
            shortestpath += graph[nowvertex].edges[nextVertex].weight;
            odwiedzone[nextVertex] = true;
            nowvertex = nextVertex;
        }
    }

    solvedpath.push_back(startvertex);//powrot do wierzcholka poczatkowego
    shortestpath += graph[nowvertex].edges[startvertex].weight;
    return shortestpath;
}

int TSP::randomMetod(std::vector<Node> graph, int V) {
    shortestpath = INT_MAX;
    std::vector<int> randomindex(V);
    std::vector<int> path;
    srand(time(0)); // Inicjalizacja generatora losowego

    for(int i = 0 ; i < V ; i++) {//indexow tyle, ile wierzcholkow
        randomindex[i] = i;
    }

    std::random_shuffle(randomindex.begin(), randomindex.end());


    path.clear();
    int lastindex = 0;
    int score = 0;

    for(int j = 0 ; j <  V; j++) {
        if(j + 1 < V) {
            score += graph[randomindex[j]].edges[randomindex[j+1]].weight;
            lastindex = j + 1;
            path.push_back(randomindex[j]);
        }
    }
    score += graph[randomindex[lastindex]].edges[randomindex[0]].weight;//laczymy ostatniego z pierwszym
    path.push_back(randomindex[V-1]);//polaczenie (n-1) - n wierz
    path.push_back(randomindex[0]);//zapewnienie pełnosci grafu
    if (score < shortestpath) {
        shortestpath = score;
        solvedpath = path;
    }

    return shortestpath;
}

