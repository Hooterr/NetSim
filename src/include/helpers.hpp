//
// Created by root on 1/3/2020.
//

#ifndef SRC_HELPERS_HPP
#define SRC_HELPERS_HPP

#include <cstdlib>
#include <iostream>
#include <random>
#include <ctime>

class Random {
public:
    static double probability_generator() {

        /* initialize random seed: */
        srand (time(NULL));


        return ((double) rand() / (RAND_MAX));
    }
};
#endif //SRC_HELPERS_HPP
