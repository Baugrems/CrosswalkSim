//
// Created by Shane on 11/13/2021.
//

#include "Simulation.h"
#include "Event.h"
#include <iostream>
#include <queue>
using namespace std;

int main() {
    priority_queue<Event> EventList;
    Welford W = Welford();
    W.v += 1;
    cout << W.v << endl;
    W.v += 12;
    cout << W.v << endl;
    return 0;
}