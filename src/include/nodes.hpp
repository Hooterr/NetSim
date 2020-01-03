//
// Created by root on 1/3/2020.
//

#ifndef SRC_NODES_HPP
#define SRC_NODES_HPP

#include <memory>
#include <optional>
#include "package.hpp"
#include "storage_types.hpp"


class IPackageReceiver
{
public:
    virtual void receive_package(Package &&p) = 0;
    virtual ElementID get_id() = 0;
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};

class ReceiverPreferences {
public:
    ReceiverPreferences(ProbabilityGeneretor pg);
    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver();
};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences;

    void send_package();
    std::optional<Package> get_sending_buffer();

protected:
    void push_package(Package && package);
};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const;
    ElementID get_id() const;

};

class Worker : public IPackageReceiver, PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> pq);
    void do_work(Time t);
    TimeOffset get_processing_duration() const;
    Time get_package_processing_start_time() const;
};

#endif //SRC_NODES_HPP
