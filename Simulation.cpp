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
#include "Welford.h"
using namespace std;


vector<float> runSim(int, string, string, string);
Pedestrian createPedestrian();
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
RandomFunctions randomFunctions = RandomFunctions("", "", "");
TrafficSignal trafficSignal = TrafficSignal();
Welford welfordPedestrians = Welford();

int main(int argc, char *argv[]) {
    std::vector<float> output = runSim(atoi(argv[1]), argv[2], argv[3], argv[4]);
    std::cout << "OUTPUT " << output.at(0) << " " << output.at(1) << std::endl;
    return 0;
}


std::vector<float> runSim(int N, string AUTO_RANDOM, string PED_RANDOM, string BUTTON_RANDOM){
    randomFunctions = RandomFunctions(AUTO_RANDOM, PED_RANDOM, BUTTON_RANDOM);
    createPedestrian();
    Event firstSignalEvent = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
    EventList.push(firstSignalEvent);
    double averageDelayPedestrian = 0;
    while (EventList.size() > 0){
        Event e = EventList.top();
        EventList.pop();
        t = e.activationTime;
        if (e.type == Event::eventType::PedArrival){
            if (numPeds < N) createPedestrian();
        }

        else if (e.type == Event::eventType::PedAtButton){
            //calculates whether or not they will press the button
            if (pedestrianAtButton(false, false, e.id)){
                buttonIsPressed = true;
            }
            trafficSignal.PedestriansAtButton.push_back(Pedestrian::getPedestrianByID(e.id));
            if(trafficSignal.pedestrianSignal == TrafficSignal::Signal::WALK){
                processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
            }
        }

        else if (e.type == Event::eventType::YellowExpires){
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            nextRedExpiration = t+trafficSignal.redTime;
            Event greenLight = Event(Event::eventType::RedExpires, t+trafficSignal.redTime);
            EventList.push(greenLight);
            //Sends any pedestrians that can cross
            processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
        }

        else if (e.type == Event::eventType::RedExpires){

            buttonIsPressed = false;
            pedestrianAtButton(false, true, -1);
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
                pedestrianAtButton(true, false, e.id);
            }
        }
        else if (e.type == Event::eventType::PedExit){
            numExit += 1;
            Pedestrian::allPedestrians.at(e.id-1).exited = true;
            welfordPedestrians.step(t - Pedestrian::allPedestrians.at(e.id-1).timeNoDelay);
        }
    }
    std::vector<float> results;
    results.push_back(welfordPedestrians.avg);
    results.push_back(welfordPedestrians.v);
    return results;
}

Pedestrian createPedestrian(){
    //TODO change randomFunctions to files
    Pedestrian ped = Pedestrian(pedID, t+randomFunctions.ExponentialPed(6), randomFunctions.UniformPed(2.6, 4.1));
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
            if (randomFunctions.UniformButton(0, 16) < 15){
                buttonIsPressed = true;
            }
        }
    }
    else{
        Event impatient = Event(Event::eventType::PedImpatient, t+60, id);
        EventList.push(impatient);

        if (PedsAtButton.empty()){
            //TODO MAKE THE UNIFORM BUTTON FILE
            if (randomFunctions.UniformButton(0, 16) < 15){
                buttonIsPressed = true;
            }
        }
        else{
            if (randomFunctions.UniformButton(0, PedsAtButton.size() + 1) < 1.0/(PedsAtButton.size() + 1) ){
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

void processNewEvents(std::vector<Event> events) {
    for (int i = 0; i < events.size(); ++i) {
        EventList.push(events.at(i));
    }
}
