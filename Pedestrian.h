//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_PEDESTRIAN_H
#define CROSSWALKSIM_PEDESTRIAN_H

#include <vector>
class Pedestrian{
public:
    Pedestrian(int id, double time, double velocity);
    static std::vector<Pedestrian> allPedestrians;
    static Pedestrian getPedestrianByID(int id);
    double velocity;
    double time;
    int id;
    bool exited;
    double timeNoDelay;
    double block = 330;
    double street = 46;
};
#endif //CROSSWALKSIM_PEDESTRIAN_H
