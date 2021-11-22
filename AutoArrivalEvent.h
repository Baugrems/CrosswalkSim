//
// Created by copen on 11/16/2021.
//

#ifndef CROSSWALKSIM_AUTOARRIVALEVENT_H
#define CROSSWALKSIM_AUTOARRIVALEVENT_H


#include "Event.h"
#include "Automobile.h"

class AutoArrivalEvent: public Event {
public:
    Automobile car;
};


#endif //CROSSWALKSIM_AUTOARRIVALEVENT_H
