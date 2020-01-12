//
// Created by root on 1/3/2020.
//

#ifndef SRC_HELPERS_HPP
#define SRC_HELPERS_HPP


#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //SRC_HELPERS_HPP
