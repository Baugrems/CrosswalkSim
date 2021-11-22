//
// Created by patri on 11/13/2021.
//

#include "RandomFunctions.h"
#include <cstdlib>
#include <math.h>
#include <iostream>


RandomFunctions::RandomFunctions(){
}


RandomFunctions::RandomFunctions(std::string pedFile, std::string autoFile, std::string buttonFile){
    this->pedFile = pedFile;
    this->autoFile = autoFile;
    this->buttonFile = buttonFile;
    pedStream.open(this->pedFile);
    autoStream.open(this->autoFile);
    buttonStream.open(this->buttonFile);
    if(pedStream.fail() || autoStream.fail() || buttonStream.fail() && !(pedFile=="" && autoFile=="" && buttonFile=="")){
	
        exit (EXIT_FAILURE);
    }
}

double RandomFunctions::ExponentialAuto(double mu) {
    float nextAutoUniform;
    if(!autoStream.eof()){
        autoStream >> nextAutoUniform;
    }
    else{
        exit (EXIT_FAILURE);
    }
    return -1 * mu * log(1 - nextAutoUniform);
}

double RandomFunctions::UniformAuto(double a, double b) {
    float nextAutoUniform;
    if(!autoStream.eof()){
        autoStream >> nextAutoUniform;
    }
    else{
        exit (EXIT_FAILURE);
    }
    return a + (b - a) * nextAutoUniform;
}


double RandomFunctions::ExponentialPed(double mu) {
    float nextPedUniform;
    if(!pedStream.eof()){
        pedStream >> nextPedUniform;
    }
    else{
        exit (EXIT_FAILURE);
    }
    return -1 * mu * log(1 - nextPedUniform);
}

double RandomFunctions::UniformPed(double a, double b) {
    float nextPedUniform;
    if(!pedStream.eof()){
        pedStream >> nextPedUniform;
    }
    else{
        exit (EXIT_FAILURE);
    }
    return a + (b - a) * nextPedUniform;
}


double RandomFunctions::UniformButton(double a, double b) {
    float nextButtonUniform;
    if(!buttonStream.eof()){
        buttonStream >> nextButtonUniform;
    }
    else{
        exit (EXIT_FAILURE);
    }
    return a + (b - a) * nextButtonUniform;
}
