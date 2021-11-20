//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_AUTOMOBILE_H
#define CROSSWALKSIM_AUTOMOBILE_H
class Automobile{
public:
    Automobile(int id, double velocity, double time);
    int length;
    int id;
    double velocity;
    double time;
    const double ACCELERATION = 10.0;
    const int LENGTH = 9;
};
#endif //CROSSWALKSIM_AUTOMOBILE_H
