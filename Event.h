//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_EVENT_H
#define CROSSWALKSIM_EVENT_H

class Event {
public:
    enum class eventType {AutoArrival, PedArrival, PedAtButton, PedImpatient, GreenExpires, YellowExpires, RedExpires, AutoExit, PedExit};
    double activationTime;
    eventType type;
    bool operator <(const Event & event) const
    {
        return activationTime < event.activationTime;
    }
};

#endif //CROSSWALKSIM_EVENT_H
