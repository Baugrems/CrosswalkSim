//
// Created by patri on 11/13/2021.
//

#include <iostream>
#include "Pedestrian.h"
std::vector<Pedestrian> Pedestrian::allPedestrians;

Pedestrian::Pedestrian(int id, double time, double velocity) {
    this->id = id;
    this->time = time;
    this->velocity = velocity;
    exited = false;
    this->timeNoDelay = time + (block+street * 2)/velocity;
}

Pedestrian Pedestrian::getPedestrianByID(int id) {
    for(auto & ped : Pedestrian::allPedestrians) {
        if (ped.id == id){
            return  ped;
        }
    }
    Pedestrian none = Pedestrian(-1, 1, 1);
    return none;
}
