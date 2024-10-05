#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <unordered_map>

#include "Node.h"
#include "File.cpp"

int travellingSalesmanProblemBF(std::vector<Node> graph, int startVertex, int V);
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

    int startVertex = 0;//wierzcholek poczatkowy
    int lowest_cost = travellingSalesmanProblemBF(graph, startVertex, V);
    std::cout <<"Minimalny koszt przejscia startujac od wierz: " << startVertex <<" to: "<< lowest_cost<<std::endl;


    return 0;
}

int travellingSalesmanProblemBF(std::vector<Node> graph, int startVertex, int V) {//V to liczba wierzcholkow
    std::vector<int> vertex;//wektor wszystkich poza pierwszym wierzcholkow

    std::vector<int> solvedpath;
    solvedpath.push_back(startVertex);//dodanie do rozwiazania wierz pocz

    for (int i = 0; i < V; i++)
        if (i != startVertex)
            vertex.push_back(i);

    int lowestcost = INT_MAX;

    do {

        int current_pathweight = 0;
        int k = startVertex;

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
        current_pathweight += graph[k].edges[startVertex].weight;
        temp_path.push_back(startVertex); // Powrót do wierzchołka startowego

        // Jeżeli bieżący koszt jest mniejszy niż dotychczasowy minimalny, aktualizujemy ścieżkę
        if (current_pathweight < lowestcost) {
            lowestcost = current_pathweight;  // Aktualizujemy minimalny koszt
            solvedpath = temp_path;           // Aktualizujemy ścieżkę, która ma najniższy koszt
        }


    } while (
        next_permutation(vertex.begin(), vertex.end()));



    for (int i = 0; i < solvedpath.size(); ++i) {//for chwilowy tylko printujacy rozwiazanie w postaci sciezki
        std::cout << solvedpath[i] << " - ";
    }

    return lowestcost;
}