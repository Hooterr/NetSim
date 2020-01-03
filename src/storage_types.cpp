//
// Created by lsriw on 12/17/19.
//
#include "include/storage_types.hpp"
#include <stdexcept>

Package& PackageQueue::pop() {
    switch(queue_type) {
        case PackageQueueType::FIFO: {
            Package& item = items.front();
            items.pop_front();
            return item;
        }
        case PackageQueueType::LIFO: {
            Package& item = items.back();
            items.pop_back();
            return item;
        }
        default:
            throw std::invalid_argument("Queue type not implemented!");

    }
}

