//
// Created by patri on 11/13/2021.
//

#include <iostream>
#include "TrafficSignal.h"

void TrafficSignal::ChangeLight() {
    switch (this->stopLightColor) {
        case Light::GREEN:
            this->stopLightColor = Light::YELLOW;
            break;
        case Light::YELLOW:
            this->stopLightColor = Light::RED;
            break;
        case Light::RED:
            this->stopLightColor = Light::GREEN;
            break;
    }
}

void TrafficSignal::ChangeCrossSignal() {
    pedestriansSent = 0;
    greenExpired = false;
    switch (this->pedestrianSignal) {
        case Signal::WALK:
            this->pedestrianSignal = Signal::NO_WALK;
            break;
        case Signal::NO_WALK:
            this->pedestrianSignal = Signal::WALK;
            break;
    }
}

TrafficSignal::TrafficSignal() {
    this->stopLightColor = Light::GREEN;
    this->pedestrianSignal = Signal::NO_WALK;
    this->pedestriansSent = 0;
}

std::vector<Event> TrafficSignal::sendPedestrians(double t, double redEnds) {
    std::vector<Event> pedExits;
    for (int i = 0; i < PedestriansAtButton.size() ; ++i) {
        if ( t + (streetLength / PedestriansAtButton.at(i).velocity) < redEnds ){
            Event pedExit = Event(Event::eventType::PedExit, t + (streetLength / PedestriansAtButton.at(i).velocity), PedestriansAtButton.at(i).id);
            PedestriansAtButton.erase(PedestriansAtButton.begin() + i);
            pedExits.push_back(pedExit);
            pedestriansSent+=1;
            i--;
        }
        if (pedestriansSent == 20){
            break;
        }
    }
    return pedExits;
}



