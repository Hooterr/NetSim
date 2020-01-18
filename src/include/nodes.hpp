//Maksymilian Lach, 302868

#ifndef SRC_NODES_HPP
#define SRC_NODES_HPP

#include <utility>
#include <memory>
#include <optional>
#include <functional>
#include <map>
#include "helpers.hpp"
#include "package.hpp"
#include "storage_types.hpp"

// Types of nodes that are able to receive a package
enum ReceiverType {
    WORKER,
    STOREHOUSE,
};

class IPackageReceiver {
public:
    virtual ElementID get_id() const = 0;
    virtual void receive_package(Package &&p) = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual ~IPackageReceiver() = default;
};


class Storehouse : public IPackageReceiver{
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO))
            : id_(id){
        sp_ = std::move(d);
    }
    void receive_package(Package &&p) override { sp_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType ::STOREHOUSE; }
    IPackageQueue::const_iterator cbegin() const { return sp_->cbegin(); }
    IPackageQueue::const_iterator cend() const { return sp_->cend(); }
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> sp_;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg) : pg_(pg) {}
    ReceiverPreferences() : pg_ (default_probability_generator) {}
    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver() const;
    const preferences_t& get_preferences() const { return preferences_; }

    // For testing
    void set_preferences(preferences_t& prefs) { preferences_ = prefs; }

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
    void rescale_probabilities();
};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() : receiver_preferences_(default_probability_generator){}
    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return buffer_; }
    PackageSender(PackageSender&&) = default;

protected:
    void push_package(Package && package);

private:
    std::optional<Package> buffer_;
};

class Ramp : public PackageSender {
public:
#define FIRST_ROUND -1
    Ramp(ElementID id, TimeOffset di) : di_(di), id_(id), start_time(FIRST_ROUND) {}
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
    ReceiverType get_receiver_type() const override{ return ReceiverType::WORKER; }
    IPackageQueue* get_queue() const { return pq_.get(); }
    IPackageStockpile::const_iterator cbegin() const { return pq_->cbegin(); }
    IPackageStockpile::const_iterator cend() const { return pq_->cend(); }
    const std::optional<Package>& get_processing_buffer() const { return buffer_; }
    TimeOffset get_time_spent(Time t) const { return t - start_time + 1; }

private:
    ElementID id_;
    TimeOffset pd_;
    Time start_time;
    std::unique_ptr<IPackageQueue> pq_;
    std::optional<Package> buffer_;
};

#endif //SRC_NODES_HPP
