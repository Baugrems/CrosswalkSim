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
#include "Automobile.h"

using namespace std;


vector<float> runSim(int, string, string, string);
Pedestrian createPedestrian();
void scheduleAtButton(Pedestrian);
bool pedestrianAtButton(bool, bool, int);
void processNewEvents(std::vector<Event>);
void startAutos();

Automobile createAuto();

priority_queue<Event> EventList;
queue<Pedestrian> PedsAtButton;
int pedID = 1;
int numPeds = 0;
int carID = 1;
int numCars = 0;
int numExit = 0;
int numCarExit = 0;
double t = 0;
double blockLength = 330;
double streetLength = 46;
double crossWidth = 24;
bool buttonIsPressed = false;
double nextRedExpiration = 0;
double lastLightChange;
RandomFunctions randomFunctions = RandomFunctions("", "", "");
TrafficSignal trafficSignal = TrafficSignal();
Welford welfordPedestrians = Welford();
Welford welfordAutos = Welford();

int main(int argc, char *argv[]) {
    std::vector<float> output = runSim(atoi(argv[1]), argv[2], argv[3], argv[4]);
    std::cout << "OUTPUT " << output.at(0) << " " << output.at(1) << std::endl;
    return 0;
}


std::vector<float> runSim(int N, string AUTO_RANDOM, string PED_RANDOM, string BUTTON_RANDOM){
    randomFunctions = RandomFunctions(AUTO_RANDOM, PED_RANDOM, BUTTON_RANDOM);
    createPedestrian();
    createAuto();
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
            lastLightChange = t;
            trafficSignal.ChangeCrossSignal();
            nextRedExpiration = t+trafficSignal.redTime;
            Event greenLight = Event(Event::eventType::RedExpires, t+trafficSignal.redTime);
            EventList.push(greenLight);
            //Sends any pedestrians that can cross
            processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
        }

        else if (e.type == Event::eventType::RedExpires){
            lastLightChange = t;
            buttonIsPressed = false;
            pedestrianAtButton(false, true, -1);
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            Event greenExpiration = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
            EventList.push(greenExpiration);
            startAutos();
        }
        else if (e.type == Event::eventType::GreenExpires){
            // TODO check for active autos to be delayed or not
            trafficSignal.greenExpired = true;
            if (buttonIsPressed) {
                lastLightChange = t;
                trafficSignal.ChangeLight();
                Event redLight = Event(Event::eventType::YellowExpires, t+trafficSignal.yellowTime);
                EventList.push(redLight);
		for(auto car: Automobile::activeAutomobiles) {
			if (t + 8 > car.ct2) {
				//exit as normal, we escaped!
			} else {
				// car is delayed
				Automobile::waitingAutos.append(car);
			}
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
        else if (e.type == Event::eventType::AutoArrival) {
            if (numCars < N) {
		    car = createAuto();
		    Event crossEvent = Event(Event::eventType::AutoCross, car.ct1);
	    }
        }
        else if (e.type == Event::eventType::AutoExit) {
            numCarExit++;
            Automobile car = Automobile::allAutomobiles.at(e.id);
            welfordAutos.step(t - car.optimalTime());
        }
    }
    std::vector<float> results;
    results.push_back(welfordPedestrians.avg);
    results.push_back(welfordPedestrians.v);
    results.push_back(welfordAutos.avg);
    results.push_back(welfordAutos.v);
    return results;
}

void startAutos() {
    for (auto car: Automobile::waitingAutos) {
        double accD = (car.velocity * car.velocity) / 20;
        double accT = car.velocity/10;
        double travelD = 1293 - accD;
        double travelT = travelD/car.velocity;

        double exitTime = t + accT + travelT;
        Event exitEvent = Event(Event::eventType::AutoExit, exitTime, car.id);
        EventList.push(exitEvent);
    }
    Automobile::waitingAutos.clear();
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

Automobile createAuto() {
    //TODO also use random file instead of this
    Automobile car = Automobile(carID, t+randomFunctions.ExponentialPed(8), randomFunctions.UniformPed(36.6667,51.3333));
    Automobile::allAutomobiles.push_back(car);
    Event autoEvent = Event(Event::eventType::AutoArrival, car.time, car.id);
    EventList.push(autoEvent);
    // figure out time to crosswalk start and end
    // distance is 1281 to first edge, 1305 to last edge
    double crossT1 = t + (1281/car.velocity);
    double crossT2 = t + (1305/car.velocity);
    car.ct1 = crossT1;
    car.ct2 = crossT2;
    carID++;
    numCars++;
    return car;
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
