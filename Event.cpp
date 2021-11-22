//
// Created by patri on 11/13/2021.
//

#include "Event.h"

Event::Event(Event::eventType type, double activationTime) {
    this->type = type;
    this->activationTime = activationTime;
}

Event::Event(Event::eventType type, double activationTime, int id) {
    this->type = type;
    this->activationTime = activationTime;
    this->id = id;
}

