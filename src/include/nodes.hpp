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
    virtual ElementID get_id() const = 0;
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id){
        sp_ = std::move(d);
    }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> sp_;
};

class ReceiverPreferences {
public:
    ReceiverPreferences(ProbabilityGeneretor pg);
    ReceiverPreferences() {} // DELETE THAT LATER
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

private:
    std::optional<Package> buffer_;
};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }

private:
    TimeOffset di_;
    ElementID id_;
    Time start_time;
};

class Worker : public IPackageReceiver, PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> pq) : id_(id), pd_(pd) {
        pq_ = std::move(pq);
    }
    void do_work(Time time);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return start_time; }
    void receive_package(Package &&p);
    ElementID get_id() const { return id_;}

private:
    ElementID id_;
    TimeOffset pd_;
    Time start_time;
    std::unique_ptr<IPackageQueue> pq_;
};

#endif //SRC_NODES_HPP
