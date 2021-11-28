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


vector<float> runSim(int);
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
int carID = 0;
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
double lastGreenLight = 99999;
TrafficSignal trafficSignal = TrafficSignal();
Welford welfordPedestrians = Welford();
Welford welfordAutos = Welford();
RandomFunctions randomFunctions = RandomFunctions();
int main(int argc, char *argv[]) {
    randomFunctions = RandomFunctions(argv[2], argv[3], argv[4]);
    std::vector<float> output = runSim(atoi(argv[1]));
    std::cout << "OUTPUT " << output.at(0) << " " << output.at(1)  << " " << output.at(2) << std::endl;
    return 0;
}


std::vector<float> runSim(int N){
    createPedestrian();
    Automobile c = createAuto();
    Event crossEvent = Event(Event::eventType::AutoCross, c.ct1, c.id);
    EventList.push(crossEvent);
    Event firstSignalEvent = Event(Event::eventType::GreenExpires, t+trafficSignal.greenTime);
    EventList.push(firstSignalEvent);
    double averageDelayPedestrian = 0;
    while (EventList.size() > 0) {
        Event e = EventList.top();
        EventList.pop();
        t = e.activationTime;
        if (e.type == Event::eventType::PedArrival) {
            if (numPeds < N) createPedestrian();
//	    std::cout << "DEBUG " << t << " PEDESTRIAN ARRIVAL " << e.id << std::endl;
        } else if (e.type == Event::eventType::PedAtButton) {
            //calculates whether or not they will press the button
            if (pedestrianAtButton(false, false, e.id)) {
                buttonIsPressed = true;
//                std::cout << "DEBUG " << t << " BUTTON IS PRESSED" << std::endl;
	    }
            trafficSignal.PedestriansAtButton.push_back(Pedestrian::getPedestrianByID(e.id));
            if (trafficSignal.pedestrianSignal == TrafficSignal::Signal::WALK) {
                processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
            }
        } else if (e.type == Event::eventType::YellowExpires) {
//            std::cout << "LIGHT TURNED RED AT " << t << std::endl;
            trafficSignal.ChangeLight();
            lastLightChange = t;
            trafficSignal.ChangeCrossSignal();
            nextRedExpiration = t + trafficSignal.redTime;
            Event greenLight = Event(Event::eventType::RedExpires, t + trafficSignal.redTime);
            EventList.push(greenLight);
            //Sends any pedestrians that can cross
            processNewEvents(trafficSignal.sendPedestrians(t, nextRedExpiration));
        } else if (e.type == Event::eventType::RedExpires) {
//            std::cout << "LIGHT TURNED GREEN AT " << t << std::endl;
            lastLightChange = t;
            buttonIsPressed = false;
            pedestrianAtButton(false, true, -1);
            trafficSignal.ChangeLight();
            trafficSignal.ChangeCrossSignal();
            Event greenExpiration = Event(Event::eventType::GreenExpires, t + trafficSignal.greenTime);
            EventList.push(greenExpiration);
            startAutos();
        } else if (e.type == Event::eventType::GreenExpires) {
            trafficSignal.greenExpired = true;
            lastGreenLight = t;
            if (buttonIsPressed) {
//                std::cout << "LIGHT TURNED YELLOW AT " << t << std::endl;
                lastLightChange = t;
                trafficSignal.ChangeLight();
                Event redLight = Event(Event::eventType::YellowExpires, t + trafficSignal.yellowTime);
                EventList.push(redLight);
            }
        } else if (e.type == Event::eventType::PedImpatient) {

            if (Pedestrian::allPedestrians.at(e.id - 1).exited) {
                continue;
            } else {
                pedestrianAtButton(true, false, e.id);
            }
        } else if (e.type == Event::eventType::PedExit) {
            numExit += 1;
//	    std::cout << "DEBUG " << t << " PED EXIT " << e.id << std::endl;
            Pedestrian::allPedestrians.at(e.id - 1).exited = true;
            welfordPedestrians.step(t - Pedestrian::allPedestrians.at(e.id - 1).timeNoDelay);
        } else if (e.type == Event::eventType::AutoArrival) {
            if (numCars < N) {
                Automobile car = createAuto();
//		std::cout << "DEBUG " << t << " AUTO ARRIVAL " << car.id << std::endl;
                Event crossEvent = Event(Event::eventType::AutoCross, car.ct2, car.id);
                EventList.push(crossEvent);
            }
        } else if (e.type == Event::eventType::AutoCross) {
            Automobile car = Automobile::allAutomobiles.at(e.id);
            if (trafficSignal.stopLightColor == TrafficSignal::Light::GREEN) {
                double accT = car.velocity/10;
                if (lastLightChange < car.ct1) {
                    Event exitEvent = Event(Event::eventType::AutoExit, car.optimalTime() + car.time, car.id);
                    EventList.push(exitEvent);
                } else {
                    double accD = (car.velocity * car.velocity) / 20;
                    double travelD = ((3.5*330)+(3*46))-accD;
                    double travelT = travelD/car.velocity;
                    double timeDelayed = 0;
//                    std::cout << car.id << " " << t << " - (" << car.ct1 << " + " << accT << ")" << std::endl;
//                    std::cout << car.id << " delayed by " << timeDelayed << std::endl;
                    double exitTime = car.time + (2 * accT) + (2 * travelT) + timeDelayed;
                    Event exitEvent = Event(Event::eventType::AutoExit, exitTime, car.id);
                    EventList.push(exitEvent);
                }
            } else if (trafficSignal.stopLightColor == TrafficSignal::Light::RED) {
                double accD = (car.velocity * car.velocity) / 20;
                double accT = car.velocity/10;
                double travelD = ((3.5*330)+(3*46)+24)-accD;
                double travelT = travelD/car.velocity;
                double timeDelayed = (lastLightChange + 18) - (car.ct1 + accT);
                // std::cout << car.id << " " << t << " - (" << car.ct1 << " + " << accT << ")" << std::endl;
                //std::cout << car.id << " delayed by " << timeDelayed << std::endl;
                double exitTime = car.time + (2 * accT) + (2 * travelT) + timeDelayed;
                Event exitEvent = Event(Event::eventType::AutoExit, exitTime, car.id);
                EventList.push(exitEvent);
            } else if (trafficSignal.stopLightColor == TrafficSignal::Light::YELLOW) {
                Event exitEvent = Event(Event::eventType::AutoExit, car.optimalTime() + car.time, car.id);
                EventList.push(exitEvent);
            }
        } else if (e.type == Event::eventType::AutoExit) {
            numCarExit++;
	    Automobile car = Automobile::allAutomobiles.at(e.id);
//	    std::cout << "DEBUG " << t << " AUTO EXIT " << e.id << " WITH A DELAY OF " << t-car.time-car.optimalTime() << std::endl;
            welfordAutos.step(t-(car.time + car.optimalTime()));
        }
    }
    std::vector<float> results;

    results.push_back(welfordAutos.avg);
    results.push_back(welfordAutos.v / N);
    results.push_back(welfordPedestrians.avg);
    return results;
}

void startAutos() {
    for (auto car: Automobile::waitingAutos) {
        //  << car.id << " delayed." << std::endl;
        double accD = (car.velocity * car.velocity) / 20;
        double accT = car.velocity/10;
        double travelD = ((3.5*330)+(3*46)+24)-accD;
        double travelT = travelD/car.velocity;
        double timeDelayed = t - (car.ct1 + accT);
        // std::cout << car.id << " " << t << " - (" << car.ct1 << " + " << accT << ")" << std::endl;
        //std::cout << car.id << " delayed by " << timeDelayed << std::endl;
        double exitTime = car.time + (2 * accT) + (2 * travelT) + timeDelayed;
        Event exitEvent = Event(Event::eventType::AutoExit, exitTime, car.id);
        EventList.push(exitEvent);
    }
    Automobile::waitingAutos.clear();
}

Pedestrian createPedestrian(){
    Pedestrian ped = Pedestrian(pedID, t+randomFunctions.ExponentialPed(10), randomFunctions.UniformPed(2.6, 4.1));
    Pedestrian::allPedestrians.push_back(ped);
    Event pedEvent = Event(Event::eventType::PedArrival, ped.time, ped.id);
    EventList.push(pedEvent);
    pedID++;
    numPeds++;
    scheduleAtButton(ped);
    return ped;
}

Automobile createAuto() {
    double speed = randomFunctions.UniformAuto(25,35);
    speed *= 1.466667;
    double accD = (speed * speed) / 20;
    double accT = speed/10;
    double crossT1 = t + (((((3.5*330)+(3*46)-12)-accD)/speed));
    double crossT2 = t + (((3.5*330)+(3*46)+12+9)/speed);
    Automobile car = Automobile(carID, speed, t+randomFunctions.ExponentialAuto(7.5), crossT1, crossT2);
    Automobile::allAutomobiles.push_back(car);
    Event autoEvent = Event(Event::eventType::AutoArrival, car.time, car.id);
    EventList.push(autoEvent);
    // figure out time to crosswalk start and end
    // distance is 1281 to first edge, 1305 to last edge
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
