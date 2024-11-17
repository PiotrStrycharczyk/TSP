//
// Created by Piotr on 15.11.2024.
//

#include "Tests.h"

#include <utility>

#include "../Algorithms/BranchNBound.h"
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

    std::cout<<"--------------------BF------------------"<<std::endl;
    Timer timer(time_limit);//obiekt przedsawiajacy stoper ma rowniez pole do przechowywanie limit poswiecony na dany alg
    // TSP brute_force(timer, shortest_path_from_file);//przekazujemy obiekt by sprawdzac ile czasu elpased
    // timer.startCounter();
    // brute_force.bruteForce(graph, V);
    // double t1 = timer.getCounter();
    // std::cout<< "Dlugosc drogi: "<<brute_force.getShortestPath() << std::endl;
    // std::cout<<"Sciezka: ";
    // for (int i : brute_force.getSolvedPath()) {
    //     std::cout << i << " ";
    // }
    // std::cout<<std::endl<<"Czas poswiecony: "<<t1<<"ms"<<std::endl;

    std::cout<<"--------------------random------------------"<<std::endl;
    TSP random(timer, shortest_path_from_file);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();
    random.randomMetod(graph, V);
    double t2 = timer.getCounter();
    std::cout<< "Dlugosc drogi: "<<random.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : random.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl<<"Czas poswiecony: "<<t2<<"ms"<<std::endl;

    std::cout<<"--------------------nn------------------"<<std::endl;
    TSP nn(timer, shortest_path_from_file);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();
    nn.repetetiveNearestNeighbour(graph, V);
    double t3 = timer.getCounter();
    std::cout<< "Dlugosc drogi: "<<nn.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : nn.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl<<"Czas poswiecony: "<<t3<<"ms"<<std::endl;

    //potrzbujemy rezultatu z metody nearestNeighbour
    int nn_resultt =  nn.repetetiveNearestNeighbour(graph, V);
    BranchNBound::nn_result = nn_resultt;//zainicjowanie pola statycznego klasy BnB

    std::cout<<"--------------------DFS------------------"<<std::endl;
    BranchNBound DFS(timer);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();
    DFS.TSPDFS(graph, V);
    double t4 = timer.getCounter();
    std::cout<< "Dlugosc drogi: "<<DFS.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : DFS.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl<<"Czas poswiecony: "<<t4<<"ms"<<std::endl;

    std::cout<<"--------------------BFS------------------"<<std::endl;
    BranchNBound BFS(timer);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();
    BFS.TSPBFS(graph, V);
    double t5= timer.getCounter();
    std::cout<< "Dlugosc drogi: "<<BFS.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : BFS.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl<<"Czas poswiecony: "<<t5<<"ms"<<std::endl;

    std::cout<<"--------------------Lowest Cost------------------"<<std::endl;
    BranchNBound LC(timer);//przekazujemy obiekt by sprawdzac ile czasu elpased
    timer.startCounter();
    LC.LowestCost(graph, V);
    double t7= timer.getCounter();
    std::cout<< "Dlugosc drogi: "<<LC.getShortestPath() << std::endl;
    std::cout<<"Sciezka: ";
    for (int i : LC.getSolvedPath()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl<<"Czas poswiecony: "<<t7<<"ms"<<std::endl;
}