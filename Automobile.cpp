//
// Created by patri on 11/13/2021.
//

#include "Automobile.h"

Automobile::Automobile(int id, double velocity, double time) {
    this->id = id;
    this->time = time;
    this->velocity = velocity;
}

double Automobile::calculate_auto_delay() {
    // Calculate the delay. If auto won't be stopped by a red light, delay is simply the distance over speed.
    // velocity * time_remaining_until_red = distance car will go.
    // If this is distance from spawn to crosswalk end + 9ft (lenght of car), easy calc.
    // Otherwise... oof
    // 18s red, 8s yellow, 35s green
    int yellow_timer = 8;
    int distToCrosswalk = 46;
    double delay = 0;
    if (this->velocity*yellow_timer >= distToCrosswalk) { // TODO this is wrong.
        // Add start time for the car so we can add that to yellow_timer as well
        delay = this->velocity / this->time;
    } else {
        //do other stuff to calc how long the car is at the crosswalk waiting.
    }
    return delay;
    //TODO
    // Also don't forget to use Welford object in Sim main to track D_A
}
