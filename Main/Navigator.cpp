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

bool stringToBool(const std::string& str) {
    if (str == "true") {
        return true;
    } else if (str == "false") {
        return false;
    }
    // Jeśli string nie jest ani "true" ani "false", możesz zwrócić domyślną wartość, np. false, albo rzucić wyjątek
    throw std::invalid_argument("Invalid input: not a valid boolean string.");
}


void printProgressBar(int current, int total, int barWidth = 50) {
    // Oblicz postęp procentowy
    float progress = (float)current / total;
    int pos = barWidth * progress;

    if(current == 0)
        std::cout<<"[";

    // Dodaj znak '=' tylko dla aktualnego postępu
    std::cout << "=";

    // Sprawdź czy to ostatnia iteracja, jeśli tak, zakończ linię
    if (current + 1 == total || current == -1) {
        std::cout << "] 100 %" << std::endl;
    }
}

int Navigator::mainProgram() {
    int V;//liczba wierzcholkow
    std::vector<Node> graph;//graf z pliku

    std::unordered_map<std::string, std::string> config = File::getConfig("../TxtFiles/conf_File.txt");//config zwracana mapa z wszystkimi klczami i wartosciami

    std::string nazwa_pliku_we = config["nazwa pliku wejsciowego"];
    std::string nazwa_pliku_wy = config["nazwa pliku wyjsciowego"];
    std::string iterations_string = config["liczba powtorzen algorytmow"];
    std::string max_time_string = config["max czas wykonywania algorytmu[ms]"];//liczba iteracji dla algorytmu losowego
    std::string showProgressBar_string = config["wyswietlanie w konsoli wskaznika postepu"];//liczba iteracji dla algorytmu losowego
    std::string iterations_randm_string = config["iteracje w random"];

    int iterations = stoi(iterations_string);
    int max_time = stoi(max_time_string);
    int iterations_randm = stoi(iterations_randm_string);

    bool showProgressBar = stringToBool(showProgressBar_string);

    std::cout << "Nazwa pliku we: " << nazwa_pliku_we << std::endl;
    std::cout << "Nazwa pliku wy: " << nazwa_pliku_wy << std::endl;
    std::cout << "Itercje algorytmow: " << iterations << std::endl;
    std::cout << "Max czas algorytmow: " << max_time << "ms" << std::endl;
    //std::cout << "Czy pokazywac progress bar " << showProgressBar << std::endl;


    //ladowanie zawartosci pliku we
    File::loadInputFile(nazwa_pliku_we, graph, V);//przekazujemy oryginal i na nim dzialamy w metodach

    //printowanie grafu
    // Counter obj1;
    // obj1.printGraph(graph, V);

    //////////////////////////////////////---brute- force - method ---///////////////////////////////////////
    //wyswietlanie danych w konsoli
    double sum_time = 0;
    double avg_time;
    std::vector<double> all_times;
    // std::cout<< "Nazwa pliku z danymi: " << nazwa_pliku_we << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Brute - Force"<<std::endl;
    TSP tspbruteforce;
    for(int j = 0 ; j < iterations; j++) {
        start_counter();
        tspbruteforce.bruteForce(graph, V);
        double t0 = get_counter();
        sum_time += t0;
        all_times.push_back(t0);//dodanie czasu do wektora

        if(showProgressBar) {
            //jesli chcemy pokazywa progressbar
            if ( j % (iterations / 50) == 0 || j + 1 == iterations) {
                printProgressBar(j, iterations, 50);
            }
        }
        if(sum_time > max_time) {
            if(showProgressBar) {//jesli szybciej skonczono
                printProgressBar(-1, iterations, 50);
            }
            avg_time = sum_time/j+1;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia czasu---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        } if(iterations - 1 == j) {
            avg_time = sum_time/iterations;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia liczby iteracji---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        }
    }

    //jako dokladna wartosc przyjmujemy wartosc srednia w bledach
    //wpisywanie do pliku /output.txt
    File f1;
    f1.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "brute - force");

    std::cout<< "Dlugosc drogi: "<< tspbruteforce.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspbruteforce.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    //////////////////////////////////////---NN - method ---///////////////////////////////////////
    sum_time = 0;
    all_times.clear();
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Nearest - Neighbour"<<std::endl;
    TSP tspRepetetiveNN;

    for(int j = 0 ; j < iterations ; j ++) {
        start_counter();
        tspRepetetiveNN.repetetiveNearestNeighbour(graph, V);
        double t1 = get_counter();
        sum_time += t1;
        all_times.push_back(t1);//dodanie czasu do wektora

        if(showProgressBar) {
            //jesli chcemy pokazywa progressbar
            if ( j % (iterations / 50) == 0 || j + 1 == iterations) {
                printProgressBar(j, iterations, 50);
            }
        }

        if(sum_time > max_time) {
            if(showProgressBar) {//jesli szybciej skonczono
                printProgressBar(-1, iterations, 50);
            }
            avg_time = sum_time/j+1;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia czasu---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        } if(iterations - 1 == j) {
            avg_time = sum_time/iterations;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia liczby iteracji---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        }
    }

    File f2;
    f2.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "nearest - neighbour");

    std::cout<< "Dlugosc drogi: "<< tspRepetetiveNN.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspRepetetiveNN.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    //////////////////////////////////////---random - method ---///////////////////////////////////////
    sum_time = 0;
    all_times.clear();

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Random Method 1"<<std::endl;
    TSP tspRandom;

   for(int j = 0 ; j < iterations ; j ++) {
        start_counter();
        tspRandom.randomMetodIterations(graph, V, iterations_randm);
        double t2 = get_counter();
        sum_time += t2;
        all_times.push_back(t2);//dodanie czasu do wektora

        if(showProgressBar) {
            //jesli chcemy pokazywa progressbar
            if ( j % (iterations / 50) == 0 || j + 1 == iterations) {
                printProgressBar(j, iterations, 50);
            }
        }

        if(sum_time > max_time) {
            if(showProgressBar) {//jesli szybciej skonczono
                printProgressBar(-1, iterations, 50);
            }
            avg_time = sum_time/j+1;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia czasu---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        } if(iterations - 1 == j) {
            avg_time = sum_time/iterations;
            double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
            double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
            std::cout<<"---Skoczenie algorytmu z powodu skonczenia liczby iteracji---" << std::endl;
            std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
            std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
            std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
            break;
        }
    }

    File f3;
    f3.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "random - method");

    std::cout<< "Dlugosc drogi: "<< tspRandom.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : tspRandom.getSolvedPath()) {
        std::cout << i << " ";
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