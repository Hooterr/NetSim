//
// Created by lsriw on 12/17/19.
//
#include "include/storage_types.hpp"


Package& PackageQueue::pop() {
    switch(queue_type) {
        case PackageQueueType::FIFO: {
            Package pck = items.front();
            items.erase(items.begin());
            return pck;
        }
        case PackageQueueType::LIFO: {
            Package pck = items.back();
            items.erase(items.end());
            return pck;
        }
    }
}


