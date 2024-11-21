//
// Created by Piotr on 15.11.2024.
//

#include "Tests.h"

#include <utility>

#include "../Algorithms/BranchNBound.h"
#include "../Algorithms/TSP.h"
#include "../Graph/GenerateGraph.h"
#include "../Main/File.cpp"
#include "../Counting/Counter.h"


Tests::Tests(bool testy_dla_pojedynczego_pliku, std::string nazwa_pliku_we, std::string nazwa_pliku_wy, bool wannaPrintInConsole, int time_limit, int iteration_limit, bool show_progress_bar, bool show_all_algorithms,
             bool show_BF, bool show_NN, bool show_random, bool show_DFS, bool show_BFS, bool show_lowest_cost)
    : if_one_instance_mode(testy_dla_pojedynczego_pliku),
      nazwa_pliku_we(std::move(nazwa_pliku_we)),
      nazwa_pliku_wy(std::move(nazwa_pliku_wy)),
      wannaPrintInConsole(wannaPrintInConsole),//czy chcemy printowac wyniki w konsoli
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
void Tests::Testy() { //testy dla brute forca
    int how_many_instances;
    std::vector<int>Vertices = {5,6,7,8,9,10,11,12,13};//wielkosci instacji do badan
    if(if_one_instance_mode) {
        how_many_instances = 1;
    } else {
        how_many_instances = Vertices.size();
    }

    int V;
    for (int v = 0; v < how_many_instances ; v++) {

        std::vector<Node> graph;//obiekt grafu z kazdym wiekszym V nowy obiekt

        if(!if_one_instance_mode) {
            V = Vertices[v];
            nazwa_pliku_we = "../TxtFiles/file_" + std::to_string(V) + ".txt";
        }

        // GenerateGraph generate_sym;
        // generate_sym.generateSymetric(graph, V);
        // Counter obj1;
        // obj1.printGraph(graph, V);

        // GenerateGraph generate_asymetric;//generowanie grafu asymetrycznego
        // generate_asymetric.generateAsymetric(graph, V);

        // Counter obj2;
        // obj2.printGraph(graph, V);
        File::loadInputFile(nazwa_pliku_we, graph, V, shortest_path_from_file);
        double avg_time = 0;
        std::vector<double>times;//wektor na poszczegolne czasy

        Timer timer(time_limit);//obiekt przedsawiajacy stoper ma rowniez pole do przechowywanie limit poswiecony na dany alg
        Timer timerPom(time_limit);//pomocniczy timer do pojedynczych pomiarow czasowych
        //-----------------BRUTE-FORCE---------------------
        if (show_all_algorithms || show_BF) {
            TSP brute_force(timer, shortest_path_from_file); // obiekt dla BF
            timer.startCounter();//przekazywany do klas TSP i BnB więc musi byc tak jak jest(odpalany przed tym foremi a w tych klasach juz ew zatrzymanie go)
            for (int i = 1; i <= iteration_limit; i++) {
                if(brute_force.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    brute_force.bruteForce(graph, V); // uruchomienie algorytmu
                    if(brute_force.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }

                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                } else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "brute - force", wannaPrintInConsole, V,
                brute_force.allPathWeights, shortest_path_from_file, brute_force.getShortestPath(), brute_force.getSolvedPath());

        }
        //czyszczenie
        avg_time = 0;
        times.clear();

        // ---------------- RANDOM METHOD ----------------
        if (show_all_algorithms || show_random) {
            TSP random(timer, shortest_path_from_file); // obiekt dla metody random
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(random.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    random.randomMetod(graph, V);
                    if(random.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
            }

            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "random", wannaPrintInConsole, V,
                random.allPathWeights,shortest_path_from_file, random.getShortestPath(), random.getSolvedPath());
        }

        //czyszczenie
        avg_time = 0;
        times.clear();

        // ---------------- REPETITIVE NEAREST NEIGHBOUR ----------------
        if (show_all_algorithms || show_NN) {
            TSP nn(timer, shortest_path_from_file); // obiekt dla NN
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(nn.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    nn.repetetiveNearestNeighbour(graph, V); // wynik NN
                    if(nn.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }
                    if(i == iteration_limit )
                        avg_time = avg_time/i;
                }else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "nn", wannaPrintInConsole, V,
                nn.allPathWeights,shortest_path_from_file,nn.getShortestPath(), nn.getSolvedPath());
        }

        //dane potrzebne do dfs
        Timer tpomm(time_limit);
        TSP nearestN(tpomm, shortest_path_from_file);
        tpomm.startCounter();
        int nn_result = nearestN.repetetiveNearestNeighbour(graph, V); // wynik NN
        //std::cout<<nn_result <<"ysys"<<std::endl;
        BranchNBound::nn_result = nn_result;//testy metod BnB sa z upperboundemNN-a

        //czyszczenie
        avg_time = 0;
        times.clear();
        //pomocniczy wektor
        std::vector<int> pom;

        // ---------------- DFS ----------------
        if (show_all_algorithms || show_DFS) {
            BranchNBound DFS(timer); // obiekt dla DFS
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(DFS.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    DFS.TSPDFS(graph, V);
                    if(DFS.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
                }
            }
            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "DFS", wannaPrintInConsole, V,
                pom,shortest_path_from_file, DFS.getShortestPath(), DFS.getSolvedPath());
        }

        //czyszczenie
        avg_time = 0;
        times.clear();

        // ---------------- BFS ----------------
        if (show_all_algorithms || show_BFS) {
            BranchNBound BFS(timer); // obiekt dla BFS
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(BFS.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    BFS.TSPBFS(graph, V);
                    if(BFS.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "BFS", wannaPrintInConsole, V,
                pom,shortest_path_from_file, BFS.getShortestPath(), BFS.getSolvedPath());
        }

        //czyszczenie
        avg_time = 0;
        times.clear();

        // ---------------- LOWEST COST ----------------
        if (show_all_algorithms || show_lowest_cost) {
            BranchNBound LC(timer); // obiekt dla Lowest Cost
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(LC.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    LC.LowestCost(graph, V);
                    if(LC.IfEndedWithIterations()) {
                        double t1 = timerPom.getCounter();
                        avg_time += t1;
                        times.push_back(t1);
                    }
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    if (times.size() == 0)
                        avg_time = 0;
                    else
                        avg_time = avg_time/(i-2);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "Lowest-cost", wannaPrintInConsole,
                V,pom,shortest_path_from_file, LC.getShortestPath(), LC.getSolvedPath());
        }
    }
}