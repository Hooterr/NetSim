//
// Created by lsriw on 1/14/20.
//

#ifndef SRC_SIMULATION_HPP
#define SRC_SIMULATION_HPP

#include <string>
#include <functional>

#include "factory.hpp"
#include "storage_types.hpp"

void simulate(Factory &factory, TimeOffset rf, std::function<void (Factory&, Time)> report);

#endif //SRC_SIMULATION_HPP
