//
// Created by root on 1/12/2020.
//
#include "factory.hpp"
#include "nodes.hpp"
#include <algorithm>
#include <map>
#include <exception>


bool Factory::is_consistent() const {

    auto colors = std::map<const PackageSender*, COLOR>();
    for(auto it = _ramps.cbegin(); it != _ramps.cend(); it++) {
        colors[dynamic_cast<const PackageSender*>(&(*it))] = COLOR::NOT_VISITED;
    }

    for(auto it = _workers.cbegin(); it != _workers.cend(); it++) {
        colors[dynamic_cast<const PackageSender*>(&(*it))] = COLOR::NOT_VISITED;
    }

    for(auto it = _ramps.cbegin(); it != _ramps.cend(); it++) {
        try{
            can_reach_storehouse(dynamic_cast<const PackageSender*>(&(*it)), colors);
        }
        catch (std::logic_error)
        {
            return false;
        }
    }

    return true;
}

bool Factory::can_reach_storehouse(const PackageSender* sender, std::map<const PackageSender*, COLOR> &colors) const
{
    if (colors[sender] == COLOR::VERIFIED)
        return true;

    colors[sender] = COLOR::VISITED;

    const std::map<IPackageReceiver*, double>& prefs =
            sender->receiver_preferences_.get_preferences();

    if(prefs.size() == 0)
        throw std::logic_error("");

    bool has_receiver_other_than_self = false;
    for(auto pair : prefs)
    {
        if (pair.first->get_receiver_type() == ReceiverType::STOREHOUSE)
        {
            has_receiver_other_than_self = true;
        }
        else
        {
            auto worker_ptr = dynamic_cast<Worker*>(pair.first);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (worker_ptr == sender)
                continue;

            has_receiver_other_than_self = true;

            if(colors[sendrecv_ptr] == COLOR::NOT_VISITED)
                can_reach_storehouse(sendrecv_ptr, colors);
        }
    }
    colors[sender] = COLOR::VERIFIED;

    if(has_receiver_other_than_self)
        return true;

    throw std::logic_error("");
}

void Factory::do_deliveries(Time t) {
    std::for_each(_ramps.begin(), _ramps.end(), [t](Ramp& x) { x.deliver_goods(t);});
}

void Factory::do_package_passing() {
    std::for_each(_ramps.begin(), _ramps.end(), [](Ramp& x) { x.send_package();});
    std::for_each(_workers.begin(), _workers.end(), [](Worker& x) { x.send_package();});
}

void Factory::do_work(Time t) {
    std::for_each(_workers.begin(), _workers.end(), [t](Worker& x) { x.do_work(t);});
}
