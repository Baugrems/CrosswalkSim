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
}
