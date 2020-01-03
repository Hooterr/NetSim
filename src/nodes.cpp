//
// Created by root on 1/3/2020.
//

#include <nodes.hpp>

void Worker::do_work(Time time) {
    if (get_sending_buffer())
    {
        if (abs(time - start_time) >= pd_)
        {
            // We're done with this package
            push_package(std::move(get_sending_buffer().value()));
        }
    }
    else
    {
        if (pq_->empty() == false) {
            Package &package = pq_->pop();
            push_package(std::move(package));
            start_time = time;
        }
    }
}

void Worker::receive_package(Package &&p) {
    pq_->push(std::move(p));
}

void Ramp::deliver_goods(Time t) {
    if(abs(start_time - t) >= di_)
    {
        push_package(Package());
        start_time = t;
    }
}

void PackageSender::send_package() {

}

std::optional<Package> PackageSender::get_sending_buffer() {
    return std::move(buffer_);
}

void PackageSender::push_package(Package &&package) {
    buffer_.emplace(std::move(package));
}



