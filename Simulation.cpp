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
bool pedestrianAtButton(bool, int);
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
RandomFunctions random = RandomFunctions();
TrafficSignal trafficSignal = TrafficSignal();
double averageDelayPedestrian = 0;
int main() {


    const int N = 9000;


    FileController UniformFiles("", "", "");
    createPedestrian(UniformFiles);
    Event firstSignalEvent = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
    EventList.push(firstSignalEvent);
    while (numExit != Pedestrian::allPedestrians.size() || Pedestrian::allPedestrians.size() < N){
        Event e = EventList.top();
        EventList.pop();
        t = e.activationTime;
        //std::cout << getEnum(e.type) << " " << t << " " << EventList.size() << std::endl;

        if (e.type == Event::eventType::PedArrival){
            if (numPeds < N) createPedestrian(UniformFiles);
        }
        else if (e.type == Event::eventType::PedAtButton){
            //calculates whether or not they will press the button
            if (pedestrianAtButton(false, e.id)){
                buttonIsPressed = true;
            }
            trafficSignal.PedestriansAtButton.push(Pedestrian::getPedestrianByID(e.id));
            //Sends any pedestrians that can cross
            if (buttonIsPressed && trafficSignal.greenExpired)
            processNewEvents(trafficSignal.sendPedestrians(t));
        }
        else if (e.type == Event::eventType::YellowExpires){
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            Event greenLight = Event(Event::eventType::RedExpires, t+trafficSignal.redTime);
            EventList.push(greenLight);
            //Sends any pedestrians that can cross
            processNewEvents(trafficSignal.sendPedestrians(t));
        }
        else if (e.type == Event::eventType::RedExpires){
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            Event greenExpiration = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
            EventList.push(greenExpiration);
        }
        else if (e.type == Event::eventType::GreenExpires){
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
                pedestrianAtButton(true, e.id);
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

bool pedestrianAtButton(bool impatientPress, int id){
    bool previousStateOfButton = buttonIsPressed;
    if (impatientPress){
        buttonIsPressed = true;
        return buttonIsPressed;
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
