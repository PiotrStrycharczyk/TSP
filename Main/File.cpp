//
// Created by Piotr on 06.10.2024.
//
#include "../Graph/Node.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>

class File { // Definicja klasy File
public:

    // Statyczna metoda zwracająca mapę klucz-wartość z pliku konfiguracyjnego
    static std::unordered_map<std::string, std::string> getConfig(const std::string& filename) {
        // std::unordered_map przechowuje konfiguracje jako pary klucz-wartość

        std::ifstream file(filename); // Otwieramy plik o nazwie podanej w filename, aby go odczytać
        std::unordered_map<std::string, std::string> config; // Tworzymy pustą mapę, która będzie przechowywać konfigurację (klucz-wartość)
        std::string line; // Zmienna do przechowywania każdej odczytanej linii z pliku

        // Pętla while, która odczytuje plik linia po linii
        while (getline(file, line)) {
            // Szukamy pozycji dwukropka (":") w aktualnej linii
            size_t pozycjaDwukropka = line.find(":");

            // Jeśli znaleziono dwukropek, czyli linia ma poprawny format klucz:wartość
            if (pozycjaDwukropka != std::string::npos) {
                // Wyciągamy klucz (tekst przed dwukropkiem)
                std::string key = line.substr(0, pozycjaDwukropka);

                // Wyciągamy wartość (tekst po dwukropku)
                std::string value = line.substr(pozycjaDwukropka + 1);//po dwukropku do konca juz

                // Zapisujemy klucz i wartość do mapy config
                config[key] = value;
            }
        }

        return config; // Zwracamy mapę config zawierającą wszystkie pary klucz-wartość
    }

    int static loadInputFile(std::string nazwa_pliku_we, std::vector<Node> & graph, int & V) {//wczytuje plik z danymi wejsciowymi
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
        return 0;
    }

    void writeTimesToOutput(std::vector<double> times, double avg_time, std::string nazwa_pliku_we, std::string nazwa_pliku_wy) {
        int n = times.size();
        double sum_relative = 0;
        double sum_absolute = 0;

        // Otwieram plik w trybie dopisywania
        std::ofstream plik;
        plik.open(nazwa_pliku_wy, std::ios::app);

        if (!plik.is_open()) {
            std::cerr << "Nie udalo sie otworzyc pliku do zapisu: " << nazwa_pliku_wy << std::endl;
            return; // Kończymy działanie funkcji, jeśli plik nie został otwarty
        }

        plik << "===========================================================" << std::endl;
        plik << "Nazwa pliku wejsciowego: " << nazwa_pliku_we << std::endl;

        plik << "LP;czas;blad bezwzgledny;blad wzgledny" << std::endl;
        for (int i = 0; i < n; i++) {
            double relative_error = fabs(times[i] - avg_time) / times[i];
            sum_relative += relative_error;
            double absolute_error = fabs(times[i] - avg_time);
            sum_absolute += absolute_error;
            plik << i + 1 << ";" << times[i] << "ms;" << relative_error << ";" << absolute_error << "ms" << std::endl;
        }

        double avg_rel_error = sum_relative / n;
        double avg_abs_error = sum_absolute / n;

        plik << "Czas sredni;bl wzgledny;bl wzgledny[%];bl bezwzgledny" << std::endl;
        plik << avg_time << "ms;" << avg_rel_error << ";" << avg_rel_error * 100 << "%;" << avg_abs_error << "ms" << std::endl;

        plik.close();
    }


};

