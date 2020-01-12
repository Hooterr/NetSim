//
// Created by root on 1/3/2020.
//

#include <nodes.hpp>

void Worker::do_work(Time time) {
    if (buffer_)
    {
        if (abs(time - start_time) >= pd_)
        {
            // We're done with this package
            push_package(std::move(buffer_.value()));
        }
    }
    else
    {
        if (pq_->empty() == false) {
            Package package = pq_->pop();
            push_package(std::move(package));
            start_time = time;
        }
    }
}

void Worker::receive_package(Package &&p) {
    pq_->push(std::move(p));
}

void Ramp::deliver_goods(Time t) {
    if(start_time == -1 || (abs(start_time - t) >= di_))
    {
        push_package(Package());
        start_time = t;
    }
}

void PackageSender::send_package() {
    if (buffer_) {
        auto receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}

std::optional<Package>& PackageSender::get_sending_buffer() {
    return buffer_;
}

void PackageSender::push_package(Package &&package) {
    buffer_.emplace(std::move(package));
}

void ReceiverPreferences::add_receiver(IPackageReceiver *receiver) {
    receivers_.insert(std::pair<IPackageReceiver*, double>(receiver, 0));
    rescale_probabilities();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *receiver) {
    receivers_.erase(receiver);
    rescale_probabilities();
}

void ReceiverPreferences::rescale_probabilities() {
    std::size_t num_of_el = receivers_.size();

    if (num_of_el == 0)
        return;

    for(auto &item : receivers_) {
        item.second = (double)1 / num_of_el;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    const double random = pg_();
        double sum = 0;
    for(auto &pair : receivers_)
    {
        sum += pair.second;
        if (random <= sum) {
            return  pair.first;
        }
    }
    throw std::length_error("No receivers to choose from.");
}


