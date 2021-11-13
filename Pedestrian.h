//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_PEDESTRIAN_H
#define CROSSWALKSIM_PEDESTRIAN_H
class Pedestrian{
public:
    Pedestrian(int id, double time, double velocity);
    double velocity;
    double time;
    int id;
};
#endif //CROSSWALKSIM_PEDESTRIAN_H
