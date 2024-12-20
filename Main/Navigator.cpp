//
// Created by Piotr on 09.10.2024.
//

#include "Navigator.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>

#include "../Graph/Node.h"
#include "File.cpp"
#include "Tests.h"
#include "../Algorithms/BranchNBound.h"
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


void printProgressBar(int current, int total, int barWidth) {
    // if(current == -1) {
    //     std::cout <<"]100%"<<std::endl;
    //     return;
    // }
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(barWidth * progress);

    // Tworzenie paska progresu w buforze
    std::cout << "\r[";

    for (int i = 0; i < barWidth; ++i) {
        if (i <= pos)
            std::cout << "=";
        else
            std::cout << " ";
    }

    // Wyświetlenie procentu ukończenia i dodanie powrotu karetki na początek linii
    std::cout << "] " << std::setw(3) << int(progress * 100.0) + 1<< "%\r";

    // Wymuszenie natychmiastowego wypisania na ekran, aby pozostawić w jednej linii
    std::cout.flush();

    // Po zakończeniu pętli, wypisz nową linię
    if (current == total - 1) {
        std::cout << std::endl;
    }
}


void printErrors(double avg_time, double BLwzgledny, double BLbezwzgledny, bool ifTimeEnded) {
    if (ifTimeEnded)
        std::cout<<"---Skoczenie algorytmu z powodu skonczenia czasu---" << std::endl;
    else
        std::cout<<"---Skoczenie algorytmu z powodu skonczenia liczby iteracji---" << std::endl;
    std::cout << "czas sredni: " << avg_time << "ms"<<std::endl;
    std::cout << "sredni blad wzgledny: " << BLwzgledny << std::endl;
    std::cout << "sredni blad wzgledny: " << BLwzgledny * 100 << "%"<<std::endl;
    std::cout << "sredni blad bezwzgledny: " << BLbezwzgledny << "ms" <<std::endl;
}

int Navigator::mainProgram() {
    int V;//liczba wierzcholkow
    std::vector<Node> graph;//graf z pliku

    std::unordered_map<std::string, std::string> config = File::getConfig("../TxtFiles/conf_File.txt");//config zwracana mapa z wszystkimi klczami i wartosciami

    std::string testy_dla_pojedynczego_pliku_string = config["czy chcesz testy dla pojedynczego pliku"];//jesli true to tylko bierze dla pliku podanego w pliku konfiguracyjnym jesli false to robi dla wsyzstkich instancji
    std::string nazwa_pliku_we = config["nazwa pliku wejsciowego"];
    std::string nazwa_pliku_wy = config["nazwa pliku wyjsciowego"];
    std::string iterations_string = config["liczba powtorzen algorytmow"];
    std::string max_time_string = config["max czas wykonywania algorytmu[ms]"];//liczba iteracji dla algorytmu losowego
    std::string showProgressBar_string = config["wyswietlanie w konsoli wskaznika postepu"];//liczba iteracji dla algorytmu losowego
    std::string show_results_in_console = config["wyswietlic wyniki algorytmow w konsoli"];
    std::string show_all_algorithms_string = config["pokazac wszystkie algorytmy"];//liczba iteracji dla algorytmu losowego
    std::string showBF_string = config["pokazac brute force"];//liczba iteracji dla algorytmu losowego
    std::string showNN_string = config["pokazac nn"];//liczba iteracji dla algorytmu losowego
    std::string show_random_string = config["pokazac random"];//liczba iteracji dla algorytmu losowego
    std::string showDFS_string = config["pokazac DFS"];//liczba iteracji dla algorytmu losowego
    std::string showBFS_string = config["pokazac BFS"];//liczba iteracji dla algorytmu losowego
    std::string show_lowest_cost_string = config["pokazac Lowest Cost"];//liczba iteracji dla algorytmu losowego


    int iterations = stoi(iterations_string);
    int max_time = stoi(max_time_string);

    bool testy_dla_pojedynczego_pliku = stringToBool(testy_dla_pojedynczego_pliku_string);
    bool showProgressBar = stringToBool(showProgressBar_string);
    bool showResults = stringToBool(show_results_in_console);
    bool show_all_algorithms = stringToBool(show_all_algorithms_string);
    bool showBF = stringToBool(showBF_string);
    bool showNN = stringToBool(showNN_string);
    bool show_random = stringToBool(show_random_string);
    bool showDFS = stringToBool(showDFS_string);
    bool showBFS = stringToBool(showBFS_string);
    bool show_lowest_cost = stringToBool(show_lowest_cost_string);

    std::cout << "Nazwa pliku we: " << nazwa_pliku_we << std::endl;
    std::cout << "Nazwa pliku wy: " << nazwa_pliku_wy << std::endl;
    std::cout << "Iteracje algorytmow: " << iterations << std::endl;
    std::cout << "Max czas algorytmow: " << max_time << "ms" << std::endl;
    //std::cout << "Czy pokazywac progress bar " << showProgressBar << std::endl;


    //ladowanie zawartosci pliku we
    //File::loadInputFile(nazwa_pliku_we, graph, V);//przekazujemy oryginal i na nim dzialamy w metodach

    Tests testBF(testy_dla_pojedynczego_pliku, nazwa_pliku_we, nazwa_pliku_wy, showResults, max_time, iterations, showProgressBar, show_all_algorithms,
        showBF, showNN, show_random, showDFS, showBFS, show_lowest_cost);
    testBF.Testy();


    //printowanie grafu
    // Counter obj1;
    // obj1.printGraph(graph, V);

    //////////////////////////////////////---brute- force - method ---///////////////////////////////////////
    //wyswietlanie danych w konsoli
    // double sum_time = 0;
    // double avg_time;
    // std::vector<double> all_times;
    // std::cout<< "Nazwa pliku z danymi: " << nazwa_pliku_we << std::endl;
    // std::cout << "---------------------------------------------" << std::endl;
    //
    //
    // std::cout << "Brute - Force"<<std::endl;
    // TSP tspbruteforce;
    //
    // for(int j = 0 ; j < iterations; j++) {
    //     start_counter();
    //     tspbruteforce.bruteForce(graph, V);
    //     double t0 = get_counter();
    //     sum_time += t0;
    //     all_times.push_back(t0);//dodanie czasu do wektora
    //
    //     if (showProgressBar) {
    //         // Aktualizacja paska progresu w każdym kroku
    //         printProgressBar(j , iterations, 50);
    //     }
    //     if(sum_time > max_time) {
    //         if(showProgressBar) {//jesli szybciej skonczono
    //             printProgressBar(-1, iterations, 50);
    //         }
    //         avg_time = sum_time/(j+1);
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, true);
    //         break;
    //     } if(iterations - 1 == j) {
    //         avg_time = sum_time/iterations;
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, false);
    //         break;
    //     }
    // }
    //
    // //jako dokladna wartosc przyjmujemy wartosc srednia w bledach
    // //wpisywanie do pliku /output.txt
    // File f1;
    // f1.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "brute - force");
    //
    // std::cout<< "Dlugosc drogi: "<< tspbruteforce.getShortestPath() << std::endl;
    // std::cout<<"Sciezka: ";
    // for (int i : tspbruteforce.getSolvedPath()) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;
    //
    // // //////////////////////////////////////---NN - method ---///////////////////////////////////////
    // sum_time = 0.0;
    // all_times.clear();
    // std::cout << "---------------------------------------------" << std::endl;
    // std::cout << "Nearest - Neighbour"<<std::endl;
    // TSP tspRepetetiveNN;
    // std::vector<bool> odwiedzone(V, false);       // Na początku wszystkie wierzchołki nieodwiedzone
    // int current_cost = 0;                         // Początkowy koszt trasy to 0
    // std::vector<int> current_path;                // Początkowa trasa jest pusta
    // std::vector<int> best_path;                   // Najlepsza trasa, którą chcemy znaleźć
    // int best_cost = INT_MAX;                      // Początkowy najlepszy koszt ustawiony na nieskończonoś
    //
    // for(int j = 0 ; j < iterations ; j ++) {
    //     start_counter();
    //     tspRepetetiveNN.repetetiveNearestNeighbour(graph, V, odwiedzone, current_cost, current_path, best_path, best_cost);
    //     double t1 = get_counter();
    //     sum_time += t1;
    //     all_times.push_back(t1);//dodanie czasu do wektora
    //
    //     if (showProgressBar) {
    //         // Aktualizacja paska progresu w każdym kroku
    //         printProgressBar(j , iterations, 50);
    //     }
    //
    //     if(sum_time > max_time) {
    //         if(showProgressBar) {//jesli szybciej skonczono
    //             printProgressBar(-1, iterations, 50);
    //         }
    //         avg_time = sum_time/(j+1);
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, true);
    //         break;
    //     } if(iterations - 1 == j) {
    //         avg_time = sum_time/iterations;
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, false);
    //         break;
    //     }
    // }
    //
    // File f2;
    // f2.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "nearest - neighbour");
    //
    // std::cout<< "Dlugosc drogi: "<< tspRepetetiveNN.getShortestPath() << std::endl;
    // std::cout<<"Sciezka: ";
    // for (int i : tspRepetetiveNN.getSolvedPath()) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;
    //
    // // TSP tspRepetetiveNN;
    // // std::vector<bool> odwiedzone(V, false);       // Na początku wszystkie wierzchołki nieodwiedzone
    // // int current_cost = 0;                         // Początkowy koszt trasy to 0
    // // std::vector<int> current_path;                // Początkowa trasa jest pusta
    // // std::vector<int> best_path;                   // Najlepsza trasa, którą chcemy znaleźć
    // // int best_cost = INT_MAX;                      // Początkowy najlepszy koszt ustawiony na nieskończonoś
    // // int wynik = tspRepetetiveNN.repetetiveNearestNeighbour(graph, V, odwiedzone, current_cost, current_path, best_path, best_cost);
    // // std::cout<<"Najlepszy cost: "<<wynik<<std::endl;
    // // std::cout<<"Najlepsza sciezka: "<<std::endl;
    // // for (int i : tspRepetetiveNN.getSolvedPath()) {
    // //     std::cout << i << " ";
    // // }
    // //////////////////////////////////////---random - method ---///////////////////////////////////////
    // sum_time = 0;
    // all_times.clear();
    //
    // std::cout << "---------------------------------------------" << std::endl;
    // std::cout << "Random Method 1"<<std::endl;
    // TSP tspRandom;
    // int bestSolRandom = 0;
    // for(int j = 0 ; j < iterations ; j ++) {
    //     start_counter();
    //     tspRandom.randomMetod(graph, V);
    //     double t2 = get_counter();
    //     bestSolRandom = tspRandom.getShortestPath();
    //     sum_time += t2;
    //     all_times.push_back(t2);//dodanie czasu do wektora
    //
    //     if (showProgressBar) {
    //         // Aktualizacja paska progresu w każdym kroku
    //         printProgressBar(j, iterations, 50);
    //     }
    //
    //     if(sum_time > max_time || bestSolRandom == tspbruteforce.getShortestPath()) {
    //         if(showProgressBar) {//jesli szybciej skonczono
    //             printProgressBar(-1, iterations, 50);
    //         }
    //         avg_time = sum_time/(j+1);
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/j+1;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/j+1;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, true);
    //         break;
    //     } if(iterations - 1 == j) {
    //         avg_time = sum_time/iterations;
    //         double BLbezwzgledny = Counter::absoluteErrorSum(all_times, avg_time)/iterations;
    //         double BLwzgledny = Counter::relativeErrorSum(all_times, avg_time)/iterations;
    //         printErrors(avg_time, BLwzgledny, BLbezwzgledny, false);
    //         break;
    //     }
    // }
    //
    // File f3;
    // f3.writeTimesToOutput(all_times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "random - method");
    //
    // std::cout<< "Dlugosc drogi: "<< tspRandom.getShortestPath() << std::endl;
    // std::cout<<"Sciezka: ";
    // for (int i : tspRandom.getSolvedPath()) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;
    //
    // //////////////////////////////////////---BnB DFS ---///////////////////////////////////////
    // std::cout << "---------------------------------------------" << std::endl;
    // std::cout<<"Branch and Bound - DFS"<<std::endl;
    // BranchNBound BnB;
    // start_counter();
    // BnB.TSPDFS(graph, V);//DFS
    // double czasBnB = get_counter();
    // std::cout<<"Oto sciezka: ";
    // for (int i : BnB.returnRoute()) {
    //     std::cout << i << " ";
    // }
    // std::cout<<std::endl<<"Oto waga najlepszej sciezki: "<<BnB.getFinalRes()<<std::endl;
    // std::cout<<"Oto czas: "<<czasBnB<<"ms"<<std::endl;

    // BranchNBound branchnbound;
    // start_counter();
    // branchnbound.TSPBFS(graph, V);
    // double czasBFS = get_counter();
    // std::cout<<std::endl;
    // std::cout<<"Oto sciezka: ";
    // for (int i : branchnbound.returnRoute()) {
    //     std::cout << i << " ";
    // }
    // std::cout<<std::endl<<"Oto waga najlepszej sciezki: "<<branchnbound.final_res<<std::endl;
    // std::cout<<"Oto czas: "<<czasBFS<<"ms"<<std::endl;

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