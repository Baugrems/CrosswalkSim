//
// Created by Shane on 11/13/2021.
//

#include "Simulation.h"
#include "Event.h"
#include "FileController.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include "RandomFunctions.h"
#include "Pedestrian.h"
#include "TrafficSignal.h"
using namespace std;

//TODO DELETE DEBUG FUNCTIONS
string getEnum(Event::eventType);


Pedestrian createPedestrian(FileController);
void scheduleAtButton(Pedestrian);
bool pedestrianAtButton(bool, bool, int);
void processNewEvents(std::vector<Event>);

priority_queue<Event> EventList;
queue<Pedestrian> PedsAtButton;
int pedID = 1;
int numPeds = 0;
int numExit = 0;
double t = 0;
double blockLength = 330;
double streetLength = 46;
bool buttonIsPressed = false;
double nextRedExpiration = 0;
RandomFunctions random = RandomFunctions();
TrafficSignal trafficSignal = TrafficSignal();
double averageDelayPedestrian = 0;
int main() {


    const int N = 300;
    srand(8);

    FileController UniformFiles("", "", "");
    createPedestrian(UniformFiles);
    Event firstSignalEvent = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
    EventList.push(firstSignalEvent);

    while (numExit != Pedestrian::allPedestrians.size() || Pedestrian::allPedestrians.size() < N){
        Event e = EventList.top();
        EventList.pop();
        t = e.activationTime;
        if((getEnum(e.type) == "PedAtButton")) std::cout << (getEnum(e.type) == "PedAtButton") << " " << t << " " << EventList.size() << std::endl;
        if (e.type == Event::eventType::PedArrival){
            if (numPeds < N) createPedestrian(UniformFiles);
        }
        else if (e.type == Event::eventType::PedAtButton){
            //calculates whether or not they will press the button
            if (pedestrianAtButton(false, false, e.id)){
                buttonIsPressed = true;
            }
            trafficSignal.PedestriansAtButton.push_back(Pedestrian::getPedestrianByID(e.id));
        }
        else if (e.type == Event::eventType::YellowExpires){
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            nextRedExpiration = t+trafficSignal.redTime;
            Event greenLight = Event(Event::eventType::RedExpires, t+trafficSignal.redTime);
            EventList.push(greenLight);
            //Sends any pedestrians that can cross
            std::cout << t << " " << trafficSignal.PedestriansAtButton.size() << " YELLOW EXPIRED\n";
            processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
        }
        else if (e.type == Event::eventType::RedExpires){
            std::cout << t << " " << trafficSignal.PedestriansAtButton.size() << " RED EXPIRED\n";

            buttonIsPressed = false;
            pedestrianAtButton(false, true, -1);
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            Event greenExpiration = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
            EventList.push(greenExpiration);
        }
        else if (e.type == Event::eventType::GreenExpires){
            std::cout << t << " " << trafficSignal.PedestriansAtButton.size() << " GREEN EXPIRED\n";

            trafficSignal.greenExpired = true;
            if (buttonIsPressed) {
                trafficSignal.ChangeLight();
                Event redLight = Event(Event::eventType::YellowExpires, t+trafficSignal.yellowTime);
                EventList.push(redLight);
            }
        }
        else if (e.type == Event::eventType::PedImpatient){
            if(Pedestrian::allPedestrians.at(e.id-1).exited){
                continue;
            }
            else{
                pedestrianAtButton(true, false, e.id);
            }
        }
        else if (e.type == Event::eventType::PedExit){
            numExit += 1;
            Pedestrian::allPedestrians.at(e.id-1).exited = true;
            averageDelayPedestrian += (t - Pedestrian::allPedestrians.at(e.id-1).timeNoDelay);
        }
    }
    std::cout << averageDelayPedestrian/N;
    return 0;
}

Pedestrian createPedestrian(FileController files){
    //TODO change random to files
    Pedestrian ped = Pedestrian(pedID, t+random.Exponential(6), random.Uniform(2.6, 4.1));
    Pedestrian::allPedestrians.push_back(ped);
    Event pedEvent = Event(Event::eventType::PedArrival, ped.time, ped.id);

    EventList.push(pedEvent);
    pedID++;
    numPeds++;
    scheduleAtButton(ped);
    return ped;
}

void scheduleAtButton(Pedestrian ped){
    Event pedAtButton = Event(Event::eventType::PedAtButton, ped.time + (blockLength+streetLength)/ped.velocity, ped.id);
    EventList.push(pedAtButton);
}

bool pedestrianAtButton(bool impatientPress, bool redExpire, int id){
    bool previousStateOfButton = buttonIsPressed;
    if (impatientPress){
        buttonIsPressed = true;
    }
    else if(redExpire){
        for (int i = 0; i < PedsAtButton.size(); ++i) {
            if (random.Uniform(0, 16) < 15){
                buttonIsPressed = true;
            }
        }
    }
    else{
        Event impatient = Event(Event::eventType::PedImpatient, t+60, id);
        EventList.push(impatient);

        if (PedsAtButton.empty()){
            //TODO MAKE THE UNIFORM BUTTON FILE
            if (random.Uniform(0, 16) < 15){
                buttonIsPressed = true;
            }
        }
        else{
            if (random.Uniform(0, PedsAtButton.size() + 1) < 1.0/(PedsAtButton.size() + 1) ){
                buttonIsPressed = true;
            }
        }
    }
    if (trafficSignal.greenExpired && buttonIsPressed && !previousStateOfButton){
        Event greenExpiresForRealThisTime = Event(Event::eventType::GreenExpires, t);
        EventList.push(greenExpiresForRealThisTime);
    }
    return buttonIsPressed;
}

void processNewEvents(std::vector<Event> events){
    for (int i = 0; i < events.size(); ++i) {
        EventList.push(events.at(i));
    }
}

string getEnum(Event::eventType event){
    switch (event){
        case Event::eventType::PedArrival:
            return "PedArrival";
        case Event::eventType::PedAtButton:
            return "PedAtButton";
        case Event::eventType::PedImpatient:
            return "PedImpatient";
        case Event::eventType::GreenExpires:
            return "GreenExpires";
        case Event::eventType::YellowExpires:
            return "YellowExpires";
        case Event::eventType::RedExpires:
            return "RedExpires";
        case Event::eventType::AutoExit:
            return "AutoExit";
        case Event::eventType::PedExit:
            return "PedExit";
    }
    return "";
}
