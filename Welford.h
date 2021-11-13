//
// Created by Shane on 11/13/2021.
//

#ifndef CROSSWALKSIM_WELFORD_H
#define CROSSWALKSIM_WELFORD_H


class Welford {
public:
    int n = 0;
    double avg = 0;
    double v = 0;

    void clear();
};



#endif //CROSSWALKSIM_WELFORD_H
