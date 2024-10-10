//
// Created by Piotr on 09.10.2024.
//

#ifndef NAVIGATOR_H
#define NAVIGATOR_H
#include <bits/chrono.h>


class Navigator {
public:
    Navigator();
    int mainProgram();
    void start_counter();
    double get_counter();

private:
    std::chrono::system_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;//pomiary czasowe
};



#endif //NAVIGATOR_H
