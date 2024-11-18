//
// Created by Piotr on 15.11.2024.
//

#ifndef TESTS_H
#define TESTS_H
#include <string>


class Tests {
public:
    Tests(std::string nazwa_pliku_we, std::string nazwa_pliku_wy, bool, int time_limit, int iteration_limit, bool show_progress_bar, bool show_all_algorithms,
        bool show_BF, bool show_NN, bool show_random, bool show_DFS, bool show_BFS, bool show_lowest_cost);

    void Testy();
    std::string nazwa_pliku_we;
    std::string nazwa_pliku_wy;
    bool wannaPrintInConsole;
    int time_limit;
    int iteration_limit;
    bool show_progress_bar;
    bool show_all_algorithms;
    bool show_BF;
    bool show_NN;
    bool show_random;
    bool show_DFS;
    bool show_BFS;
    bool show_lowest_cost;
    int shortest_path_from_file = INT_MAX;//pole tej klasy jest przekazywana jego referencja do funkcji LoadinputFile
    //przypisujemy INT_MAX aby w razie niepowodzenia nadpisania pozniej tej zmiennej program sie nie wysypywal


private:

};



#endif //TESTS_H
