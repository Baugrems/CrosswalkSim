//
// Created by patri on 11/13/2021.
//

#include "FileController.h"

FileController::FileController(std::string automobile, std::string ped, std::string button) {
    this->AUTO_RANDOM = automobile;
    this->PED_RANDOM = ped;
    this->BUTTON_RANDOM = button;
}

double FileController::GetRandomAuto() {
    return (double)rand()/RAND_MAX;
}

double FileController::GetRandomPed() {
    return (double)rand()/RAND_MAX;
}

double FileController::GetRandomButton() {
    return (double)rand()/RAND_MAX;
}
