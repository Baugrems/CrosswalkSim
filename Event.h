//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_EVENT_H
#define CROSSWALKSIM_EVENT_H

class Event {
public:
    enum class eventType {AutoArrival, AutoCross, PedArrival, PedAtButton, PedImpatient, GreenExpires, YellowExpires, RedExpires, AutoExit, PedExit};
    double activationTime;
    eventType type;
    int id;
    Event(eventType, double);
    Event(eventType, double, int);
    bool operator <(const Event & event) const
    {
        return activationTime > event.activationTime;
    }
};

#endif //CROSSWALKSIM_EVENT_H
