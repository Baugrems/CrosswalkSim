//
// Created by Shane on 11/13/2021.
//

#include "Simulation.h"
#include <iostream>

using namespace std;

int main() {
    Welford W = Welford();
    W.v += 1;
    cout << W.v << endl;
    W.v += 12;
    cout << W.v << endl;
    return 0;
}