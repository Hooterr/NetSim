//
// Created by lsriw on 12/17/19.
//

#ifndef SRC_STORAGE_TYPES_HPP
#define SRC_STORAGE_TYPES_HPP

#include <stdlib.h>
#include <list>
#include "package.hpp"

enum class PackageQueueType{
    FIFO,
    LIFO,
};

class IPackageStockpile{
public:
    virtual void push(Package&& package)=0;
    virtual bool empty() const=0;
    virtual std::size_t size() const=0;
};

class IPackageQueue : public IPackageStockpile{
public:
    virtual Package& pop()=0;
    virtual PackageQueueType get_queue_type() const=0;
};

class PackageQueue : public IPackageQueue{
private:
    std::list<Package> items;
    PackageQueueType queue_type;
public:
    using const_iter = std::list<Package>::const_iterator;
    std::size_t size() const  {
        return items.size();
    }

    bool empty() const  {
        return items.empty();
    }

    void push(Package &&package)  {
        items.emplace_back(std::move(package));
    }

    PackageQueueType get_queue_type() const {
        return queue_type;
    }

    Package& pop();

    using iterator = std::list<Package>::const_iterator;
    explicit PackageQueue(PackageQueueType queueType) : queue_type(queueType){}

};

#endif //SRC_STORAGE_TYPES_HPP
