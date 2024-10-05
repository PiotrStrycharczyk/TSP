//
// Created by Piotr on 06.10.2024.
//

#include <unordered_map>
#include <fstream>
#include <string>

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

};

