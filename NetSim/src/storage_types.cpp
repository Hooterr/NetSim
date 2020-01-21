//Maksymilian Lach, 302868
//
// Created by lsriw on 12/17/19.
//
#include "storage_types.hpp"
#include <stdexcept>

Package PackageQueue::pop() {
    switch(queue_type) {
        case PackageQueueType::FIFO: {
            Package item = std::move(items.front());
            items.pop_front();
            return item;
        }
        case PackageQueueType::LIFO: {
            Package item = std::move(items.back());
            items.pop_back();
            return item;
        }
        default:
            throw std::invalid_argument("Queue type not implemented!");
    }
}
