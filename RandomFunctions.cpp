//
// Created by patri on 11/13/2021.
//

#include "RandomFunctions.h"
#include <cstdlib>
#include <math.h>
#include <iostream>

RandomFunctions::RandomFunctions(std::string pedFile, std::string autoFile, std::string buttonFile){
    this->pedFile = pedFile;
    this->autoFile = autoFile;
    this->buttonFile = buttonFile;
    pedStream.open(this->pedFile);
    autoStream.open(this->autoFile);
    buttonStream.open(this->buttonFile);
}


double RandomFunctions::ExponentialPed(double mu) {
    float nextPedUniform;
    if(!pedStream.eof()){
        pedStream >> nextPedUniform;
    }
    return -1 * mu * log(1 - nextPedUniform);
}

double RandomFunctions::UniformPed(double a, double b) {
    float nextPedUniform;
    if(!pedStream.eof()){
        pedStream >> nextPedUniform;
    }
    return a + (b - a) * nextPedUniform;
}


double RandomFunctions::UniformButton(double a, double b) {
    float nextButtonUniform;
    if(!buttonStream.eof()){
        buttonStream >> nextButtonUniform;
    }
    return a + (b - a) * nextButtonUniform;
}