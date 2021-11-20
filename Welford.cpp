//
// Created by Shane on 11/13/2021.
//

#include "Welford.h"
#include <math.h>

void Welford::clearAll() {
    this->n = 0;
    this->avg = 0;
    this->v = 0;
}

void Welford::step(double x) {
    n++;
    avg += (1.0/n) * (x - avg);
    v += (n-1.0)/n * pow(x - avg, 2);
}