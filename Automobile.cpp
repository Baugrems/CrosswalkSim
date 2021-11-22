//
// Created by patri on 11/13/2021.
//

#include "Automobile.h"

std::vector<Automobile> Automobile::allAutomobiles;

std::vector<Automobile> Automobile::waitingAutos;
;
Automobile::Automobile(int id, double velocity, double time) {
    this->id = id;
    this->time = time;
    this->velocity = velocity;
}

double Automobile::optimalTime() {
    return 2586 / this->velocity;
}

