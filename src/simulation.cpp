//
// Created by lsriw on 1/14/20.
//

#include "simulation.hpp"

void simulate(Factory &factory, TimeOffset rf, std::function<void (Factory&, Time)> report) {
    if (!factory.is_consistent())
        throw std::logic_error("Giver network is not consistent.");

    for (Time t= 1; t <= rf; t++){
        factory.do_deliveries(t);
        factory.do_package_passing();
        factory.do_work(t);
        report(factory, t);
    }

}