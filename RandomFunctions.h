//
// Created by patri on 11/13/2021.
//
#include <stdlib.h>
#include <fstream>
#ifndef CROSSWALKSIM_RANDOMFUNCTIONS_H
#define CROSSWALKSIM_RANDOMFUNCTIONS_H

class RandomFunctions{
public:
    RandomFunctions(std::string, std::string, std::string);
    double ExponentialPed(double mu);
    double UniformPed(double a, double b);
    double UniformButton(double a, double b);
    double UniformAuto(double a, double b);
    double ExponentialAuto(double mu);
private:
    std::string pedFile;
    std::string autoFile;
    std::string buttonFile;
    std::ifstream pedStream;
    std::ifstream autoStream;
    std::ifstream buttonStream;
};
#endif //CROSSWALKSIM_RANDOMFUNCTIONS_H
