//
// Created by lsriw on 1/14/20.
//

#ifndef SRC_REPORTS_HPP
#define SRC_REPORTS_HPP

#include <string>
#include <functional>
#include <set>

#include "storage_types.hpp"
#include "factory.hpp"

class IntervalReportNotifier {
public:
    IntervalReportNotifier(TimeOffset _time_offset) : _time_offset(_time_offset) {}
    bool should_generate_report(Time t) { return t == _time_offset; }
private:
    TimeOffset _time_offset;
};

class SpecificTurnsReportNotifier {
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : _turns(turns) {}
    bool should_generate_report(Time t) { return _turns.find(t) != _turns.end(); }

private:
    std::set<Time> _turns;
};

void generate_structure_report(Factory &factory, std::ostream &os);
void generate_simulation_turn_report(Factory &factory, std::ostream &os, Time t);

#endif //SRC_REPORTS_HPP
