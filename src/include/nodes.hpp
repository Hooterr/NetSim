#include <utility>

//
// Created by root on 1/3/2020.
//

#ifndef SRC_NODES_HPP
#define SRC_NODES_HPP

#include <memory>
#include <optional>
#include <functional>
#include <map>
#include "helpers.hpp"
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
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO))
            : id_(id){
        sp_ = std::move(d);
    }
    void receive_package(Package &&p) override { sp_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> sp_;
};

class ReceiverPreferences {
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg) : pg_(pg) {}
    ReceiverPreferences() : pg_ (default_probability_generator) {}
    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver();
    const std::map<IPackageReceiver*, double>& get_preferences() const { return receivers_; }
private:
    ProbabilityGenerator pg_;
    std::map<IPackageReceiver*, double> receivers_;
    void rescale_probabilities();
};

class PackageSender {
public:
    PackageSender() : receiver_preferences_(default_probability_generator){}
    ReceiverPreferences receiver_preferences_;
    void send_package();
    std::optional<Package>& get_sending_buffer();
protected:
    void push_package(Package && package);

private:
    std::optional<Package> buffer_;
};

class Ramp : public PackageSender{
public:
#define FIRST_ROUND -1
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di), start_time(FIRST_ROUND) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }

private:
    TimeOffset di_;
    ElementID id_;
    Time start_time;
};

class Worker : public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> pq) : id_(id), pd_(pd), start_time(1) {
        pq_ = std::move(pq);
    }
    void do_work(Time time);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return start_time; }
    void receive_package(Package &&p) override;
    ElementID get_id() const override { return id_;}

private:
    ElementID id_;
    TimeOffset pd_;
    Time start_time;
    std::unique_ptr<IPackageQueue> pq_;
    std::optional<Package> buffer_;
};

#endif //SRC_NODES_HPP
