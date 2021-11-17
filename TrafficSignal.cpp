//
// Created by patri on 11/13/2021.
//

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

std::vector<Event> TrafficSignal::sendPedestrians(double t) {
    std::vector<Event> pedExits;
    while (pedestriansSent < 20 && !PedestriansAtButton.empty()){
        Event pedExit = Event(Event::eventType::PedExit, t + streetLength / PedestriansAtButton.front().velocity, PedestriansAtButton.front().id);
        PedestriansAtButton.pop();
        pedExits.push_back(pedExit);
    }
    return pedExits;
}



