//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_TRAFFICSIGNAL_H
#define CROSSWALKSIM_TRAFFICSIGNAL_H
class TrafficSignal{
    public:
    enum class Light {GREEN, YELLOW, RED};
    enum class Signal {WALK, NO_WALK};
    Light stopLightColor;
    Signal pedestrianSignal;
    TrafficSignal();
    void ChangeLight();
    void ChangeCrossSignal();
};
#endif //CROSSWALKSIM_TRAFFICSIGNAL_H
