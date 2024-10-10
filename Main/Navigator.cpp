//
// Created by Piotr on 09.10.2024.
//

#include "Navigator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>

#include "../Graph/Node.h"
#include "File.cpp"
#include "../Algorithms/TSP.h"
#include "../Counting/Counter.h"

Navigator::Navigator() {//konstruktor
}

int Navigator::mainProgram() {
    int V;//liczba wierzcholkow
    std::vector<Node> graph;//graf z pliku

    std::unordered_map<std::string, std::string> config = File::getConfig("../TxtFiles/conf_File.txt");//config zwracana mapa z wszystkimi klczami i wartosciami

    std::string nazwa_pliku_we = config["nazwa pliku wejsciowego"];
    std::string nazwa_pliku_wy = config["nazwa pliku wyjsciowego"];
    std::string iterations_string = config["liczba powtorzen algorytmow"];
    std::string max_time_string = config["max czas wykonywania algorytmu"];//liczba iteracji dla algorytmu losowego

    int iterations = stoi(iterations_string);
    int max_time = stoi(max_time_string);

    std::cout << "Nazwa pliku we: " << nazwa_pliku_we << std::endl;
    std::cout << "Nazwa pliku wy: " << nazwa_pliku_wy << std::endl;
    std::cout << "Itercje algorytmow: " << iterations << std::endl;
    std::cout << "Max czas algorytmow: " << max_time << "ms" << std::endl;



    // Otwieramy plik
    std::ifstream plik(nazwa_pliku_we);

    // Sprawdzamy, czy udało się otworzyć plik
    if (!plik.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku: " << nazwa_pliku_we << std::endl;
        return 1; // Zwracamy kod błędu
    }


    if (plik >> V) {

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

    //printowanie grafu
    // Counter obj1;
    // obj1.printGraph(graph, V);


    //wyswietlanie danych w konsoli
    double sum_time = 0;
    double avg_time;
    std::vector<double> all_times;
    std::cout << "==============================================" << std::endl;
    std::cout << "Brute - Force"<<std::endl;
    TSP tspbruteforce;
    for(int j = 0 ; j < iterations; j++) {
        start_counter();
        tspbruteforce.bruteForce(graph, V);
        double t0 = get_counter();
        sum_time += t0;
        all_times.push_back(t0);//dodanie czasu do wektora
        if(sum_time > max_time) {
            avg_time = sum_time/j+1;
            std::cout<<"Skoczenie algorytmu z powodu skonczenia czasu" << std::endl;
            break;
        } else if(iterations - 1 == j) {
            avg_time = sum_time/iterations;
            std::cout<<"Skoczenie algorytmu z powodu skonczenia liczby iteracji" << std::endl;
            break;
        }
    }

    //jako dokladna wartosc przyjmujemy wartosc srednia w bledach

    Counter counter;
    counter.printTimes(all_times, avg_time);

    std::cout<< "Dlugosc drogi: "<< tspbruteforce.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspbruteforce.getSolvedPath()) {
        std::cout << i << " - ";
    }
    std::cout << std::endl;


    std::cout << "==============================================" << std::endl;
    std::cout << "Nearest - Neighbour"<<std::endl;
    TSP tspNN;

    start_counter();
    tspNN.nearestNeighbour(graph, V);
    double t1 = get_counter();
    std:: cout<<"Czas wykonania algorytmu: " << t1 <<"ms"<<std::endl;

    std::cout<< "Dlugosc drogi: "<< tspNN.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspNN.getSolvedPath()) {
        std::cout << i << " - ";
    }
    std::cout << std::endl;

    std::cout << "==============================================" << std::endl;
    std::cout << "Random Method"<<std::endl;
    TSP tspR;

    start_counter();
    tspR.randomMetod(graph, V);
    double t2 = get_counter();
    std:: cout<<"Czas wykonania algorytmu: " << t2 <<"ms"<<std::endl;

    std::cout<< "Dlugosc drogi: "<< tspR.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspR.getSolvedPath()) {
        std::cout << i << " - ";
    }
    std::cout << std::endl;



    return 0;
}

void Navigator::start_counter() {
    // Ustawienie startowego czasu
    start = std::chrono::high_resolution_clock::now();
}

double Navigator::get_counter() {
    end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();//zwraca czas po przecinku w milisekundach
}