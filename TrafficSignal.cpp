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
    bool noPedLeft = false;
//    for (int i = 0; i < PedestriansAtButton.size(); ++i) {
//        std::cout << PedestriansAtButton.at(i).id << " Makes it across at: " << t + streetLength / PedestriansAtButton.at(i).velocity << " Red Ends at: " << redEnds << std::endl;
//    }
    while (pedestriansSent < 20 && !PedestriansAtButton.empty() && !noPedLeft){
        if (t + streetLength / PedestriansAtButton.front().velocity < redEnds){
            Event pedExit = Event(Event::eventType::PedExit, t + streetLength / PedestriansAtButton.front().velocity, PedestriansAtButton.front().id);
            PedestriansAtButton.erase(PedestriansAtButton.begin());
            pedExits.push_back(pedExit);
            pedestriansSent+=1;
            //std::cout << "FIRST IN LINE: " << pedExit.id << " AT TIME: " << t << " FINISH CROSS AT: " << t + streetLength / PedestriansAtButton.front().velocity << " RED LIGHT ENDS AT: " << redEnds << std::endl;
        }
        else{
            //std::cout << "CUTTING PERMITTED\n";
            for (int i = 0; i < PedestriansAtButton.size() ; ++i) {
                if (t + streetLength / PedestriansAtButton.at(i).velocity < redEnds){
                    Event pedExit = Event(Event::eventType::PedExit, t + streetLength / PedestriansAtButton.at(i).velocity, PedestriansAtButton.at(i).id);
                    PedestriansAtButton.erase(PedestriansAtButton.begin() + i);
                    pedExits.push_back(pedExit);
                    pedestriansSent+=1;
                    //std::cout << "CUT IN LINE: " << pedExit.id << " AT TIME: " << t << " FINISH CROSS AT: " << t + streetLength / PedestriansAtButton.front().velocity << " RED LIGHT ENDS AT: " << redEnds << std::endl;
                    break;
                }
                if (i==PedestriansAtButton.size()-1)noPedLeft = true;
            }

        }
    }
//    for (int i = 0; i < PedestriansAtButton.size(); ++i) {
//        std::cout << PedestriansAtButton.at(i).id << std::endl;
//    }
    return pedExits;
}



