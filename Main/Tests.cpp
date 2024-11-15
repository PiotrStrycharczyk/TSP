//
// Created by Piotr on 15.11.2024.
//

#include "Tests.h"

#include <utility>
#include "../Algorithms/TSP.h"
#include "../Main/File.cpp"


Tests::Tests(std::string nazwa_pliku_we, int time_limit, int iteration_limit, bool show_progress_bar, bool show_all_algorithms,
             bool show_BF, bool show_NN, bool show_random, bool show_DFS, bool show_BFS, bool show_lowest_cost)
    : nazwa_pliku_we(std::move(nazwa_pliku_we)),
      time_limit(time_limit),
      iteration_limit(iteration_limit),
      show_progress_bar(show_progress_bar),
      show_all_algorithms(show_all_algorithms),
      show_BF(show_BF),
      show_NN(show_NN),
      show_random(show_random),
      show_DFS(show_DFS),
      show_BFS(show_BFS),
      show_lowest_cost(show_lowest_cost)
{
    // Tutaj można dodać dodatkową logikę w ciele konstruktora, jeśli to konieczne.
}
void Tests::TestBT() { //testy dla brute forca
    int V;
    std::vector<Node> graph;
    File::loadInputFile(nazwa_pliku_we, graph, V, shortest_path_from_file);

    Timer timer(time_limit);
    TSP brute_force(timer, shortest_path_from_file);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();

    brute_force.bruteForce(graph, V);
    std::cout<< "Dlugosc drogi: "<<brute_force.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : brute_force.getSolvedPath()) {
        std::cout << i << " ";
    }

}