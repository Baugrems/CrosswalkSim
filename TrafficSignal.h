//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_TRAFFICSIGNAL_H

#include <queue>
#include "Event.h"
#include "Pedestrian.h"
#define CROSSWALKSIM_TRAFFICSIGNAL_H
class TrafficSignal{
    public:
    enum class Light {GREEN, YELLOW, RED};
    enum class Signal {WALK, NO_WALK};
    std::vector<Pedestrian> PedestriansAtButton;
    Light stopLightColor;
    Signal pedestrianSignal;
    TrafficSignal();
    std::vector<Event> sendPedestrians(double t, double redEnds);
    void ChangeLight();
    void ChangeCrossSignal();
    int pedestriansSent;
    const int greenTime = 35;
    const int yellowTime = 8;
    const int redTime = 18;
    double blockLength = 330;
    double streetLength = 46;
    bool greenExpired = false;
};
#endif //CROSSWALKSIM_TRAFFICSIGNAL_H
