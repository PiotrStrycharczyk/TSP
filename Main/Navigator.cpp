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


    //ladowanie zawartosci pliku we
    File::loadInputFile(nazwa_pliku_we, graph, V);//przekazujemy oryginal i na nim dzialamy w metodach

    //printowanie grafu
    // Counter obj1;
    // obj1.printGraph(graph, V);


    //wyswietlanie danych w konsoli
    double sum_time = 0;
    double avg_time;
    std::vector<double> all_times;
    std::cout<< "Nazwa pliku z danymi: " << nazwa_pliku_we << std::endl;
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
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
            std::cout<<"Skoczenie algorytmu z powodu skonczenia czasu" << std::endl;
            std::cout << "czas;bl wzgledny;bl wzgledny[%];bl bezwzgledny" << std::endl;//nie wyrazamy bezwzglednego w %
            std::cout << avg_time <<";" << BLwzgledny <<";"<<BLwzgledny*100<<"%:"<<BLbezwzgledny<<std::endl;
            break;
        } if(iterations - 1 == j) {
            avg_time = sum_time/iterations;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
            std::cout<<"Skoczenie algorytmu z powodu skonczenia liczby iteracji" << std::endl;
            std::cout << "czas;bl wzgledny;bl wzgledny[%];bl bezwzgledny" << std::endl;//nie wyrazamy bezwzglednego w %
            std::cout << avg_time <<"[ms];" << BLwzgledny <<";"<<BLwzgledny*100<<"%:"<<BLbezwzgledny<<"[ms]"<<std::endl;
            break;
        }
    }

    //jako dokladna wartosc przyjmujemy wartosc srednia w bledach
    //wpisywanie do pliku /output.txt
    File f1;
    f1.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy);

    std::cout<< "Dlugosc drogi: "<< tspbruteforce.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspbruteforce.getSolvedPath()) {
        std::cout << i << " - ";
    }
    std::cout << std::endl;


    std::cout << "==============================================" << std::endl;
    std::cout << "Nearest - Neighbour"<<std::endl;
    TSP tspRepetetiveNN;

    start_counter();
    tspRepetetiveNN.repetetiveNearestNeighbour(graph, V);
    double t1 = get_counter();
    std:: cout<<"Czas wykonania algorytmu: " << t1 <<"ms"<<std::endl;

    std::cout<< "Dlugosc drogi: "<< tspRepetetiveNN.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspRepetetiveNN.getSolvedPath()) {
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