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


Tests::Tests(std::string nazwa_pliku_we, std::string nazwa_pliku_wy, bool wannaPrintInConsole, int time_limit, int iteration_limit, bool show_progress_bar, bool show_all_algorithms,
             bool show_BF, bool show_NN, bool show_random, bool show_DFS, bool show_BFS, bool show_lowest_cost)
    : nazwa_pliku_we(std::move(nazwa_pliku_we)),
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
    std::vector<int>Vertices = {9,10,11,12,13,14};//wielkosci instacji do badan
    for (int v = 0; v < Vertices.size() ; v++) {

        std::vector<Node> graph;//obiekt grafu
        int V = Vertices[v];

        // GenerateGraph generate_sym;
        // generate_sym.generateSymetric(graph, V);
        // Counter obj1;
        // obj1.printGraph(graph, V);

        GenerateGraph generate_asymetric;//generowanie grafu asymetrycznego
        generate_asymetric.generateAsymetric(graph, V);

        // Counter obj2;
        // obj2.printGraph(graph, V);
        //File::loadInputFile(nazwa_pliku_we, graph, V, shortest_path_from_file);
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
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                } else {
                    avg_time = avg_time/(i-1);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "brute - force", wannaPrintInConsole, V);

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
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                avg_time = avg_time/(i-1);
                break;
            }

            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "random", wannaPrintInConsole, V);
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
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    avg_time = avg_time/(i-1);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "nn", wannaPrintInConsole, V);
        }

        //dane potrzebne do dfs
        TSP nearestN(timer, shortest_path_from_file);
        int nn_result = nearestN.repetetiveNearestNeighbour(graph, V); // wynik NN
        BranchNBound::nn_result = nn_result;

        //czyszczenie
        avg_time = 0;
        times.clear();

        // ---------------- DFS ----------------
        if (show_all_algorithms || show_DFS) {
            BranchNBound DFS(timer); // obiekt dla DFS
            timer.startCounter();
            for(int i = 1; i <= iteration_limit; i++) {
                if(DFS.IfEndedWithIterations()) {
                    timerPom.startCounter();
                    DFS.TSPDFS(graph, V);
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    avg_time = avg_time/(i-1);
                    break;
                }
            }
            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "DFS", wannaPrintInConsole, V);
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
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    avg_time = avg_time/(i-1);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "BFS", wannaPrintInConsole, V);
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
                    double t1 = timerPom.getCounter();
                    avg_time += t1;
                    times.push_back(t1);
                    if(i == iteration_limit)
                        avg_time = avg_time/i;
                }else {
                    avg_time = avg_time/(i-1);
                    break;
                }
            }

            File f1;
            f1.writeTimesToOutput(times, avg_time, nazwa_pliku_we, nazwa_pliku_wy, "Lowest-cost", wannaPrintInConsole, V);
        }
    }
}