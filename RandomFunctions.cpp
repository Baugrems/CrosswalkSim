//
// Created by patri on 11/13/2021.
//

#include "RandomFunctions.h"
#include <cstdlib>
#include <math.h>

double Random() {
    return (double) rand() / (RAND_MAX);
}

double RandomFunctions::Exponential(double mu) {
    return -1 * mu * log(1 - Random());
}

double RandomFunctions::Uniform(double a, double b) {
    return a + (b - a) * Random();
}
