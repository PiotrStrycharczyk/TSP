#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <unordered_map>

#include "../Graph/Node.h"
#include "File.cpp"
#include "../Algorithms/TSP.h"

int main()
{
    int V;//liczba wierzcholkow
    std::vector<Node> graph;//graf z pliku

    std::unordered_map<std::string, std::string> config = File::getConfig("conf_File.txt");//config zwracana mapa z wszystkimi klczami i wartosciami

    std::string nazwa_pliku_we = config["nazwa pliku wejsciowego"];
    //std::cout << "Nazwa pliku wyjsciowego: " << config["nazwa pliku wyjsciowego"] << std::endl;
    std::string type_of_problem = config["rodzaj rowiazania problemu"];

    std::cout << "Nazwa pliku we: " << nazwa_pliku_we << std::endl;
    std::cout << "Rodzaj problemu: " << type_of_problem << std::endl << std::endl;

    // Otwieramy plik
    std::ifstream plik(nazwa_pliku_we);

    // Sprawdzamy, czy udało się otworzyć plik
    if (!plik.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku: " << nazwa_pliku_we << std::endl;
        return 1; // Zwracamy kod błędu
    }


    if (plik >> V) {
        std::cout << "Liczba wierzcholkow: " << V << std::endl;
    } else {
        std::cerr << "Błąd podczas odczytu pierwszej liczby." << std::endl;
        return 1;
    }

    // Tworzymy V wierzchołków
    for (int i = 0; i < V; i++) {
        graph.push_back(Node(i)); // Numerujemy wierzchołki od 0 do V-1
    }

    int weight;//;//liczba ktora jest defact waga krawedzi miedzy Nodeami
    for(int i = 0; i < V ; i++) {
        for(int j = 0 ; j < V; j++) {
            if(plik >> weight) {
                //if (weight >= 0) {
                    graph[i].addEdge(j, weight);
                    //graph[i].neighbourCount++;
                //}
            } else
                std::cout<<"Blad odczytu danej"<<std::endl;

        }
    }

    plik.close();

    for(int i = 0 ; i < V ; i++) {//wiersze
        for (int j = 0 ; j < V ; j++) {//kolumny
            std::cout << graph[i].edges[j].weight << " ";
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl;

    // TSP tsp;
    // tsp.bruteForce(graph, V);
    // std::cout<< "To jest droga: "<< tsp.getShortestPath() << std::endl;
    // std::cout<<"A to sciezka: ";
    // for (int i : tsp.getSolvedPath()) {
    //     std::cout << i << " - ";
    // }
    // std::cout << std::endl;

    // TSP tspNN;
    // tspNN.nearestNeighbour(graph, V);
    // std::cout<< "To jest droga: "<< tspNN.getShortestPath() << std::endl;
    // std::cout<<"A to sciezka: ";
    // for (int i : tspNN.getSolvedPath()) {
    //     std::cout << i << " - ";
    // }
    // std::cout << std::endl;

    TSP tspR;
    tspR.randomMetod(graph, V, 100);
    std::cout<< "To jest droga: "<< tspR.getShortestPath() << std::endl;
    std::cout<<"A to sciezka: ";
    for (int i : tspR.getSolvedPath()) {
        std::cout << i << " - ";
    }
    std::cout << std::endl;

    return 0;
}
