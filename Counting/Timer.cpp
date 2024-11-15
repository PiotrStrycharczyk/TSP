
#include <chrono>

class Timer {
public:
    Timer(double limit) : time_limit(limit){

    }

    double time_limit;
    std::chrono::system_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;//pomiary czasowe

    void startCounter() {
        // Ustawienie startowego czasu
        start = std::chrono::high_resolution_clock::now();
    }

    double getCounter() {
        end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();//zwraca czas po przecinku w milisekundach
    }

};
