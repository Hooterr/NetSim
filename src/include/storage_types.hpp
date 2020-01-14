//Maksymilian Lach, 302868
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
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&& package)=0;
    virtual bool empty() const=0;
    virtual std::size_t size() const=0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};

class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop()=0;
    virtual PackageQueueType get_queue_type() const=0;
};

class PackageQueue : public IPackageQueue{
private:
    std::list<Package> items;
    PackageQueueType queue_type;
public:
    std::size_t size() const override {
        return items.size();
    }

    bool empty() const override {
        return items.empty();
    }

    void push(Package &&package) override {
        items.emplace_back(std::move(package));
    }

    PackageQueueType get_queue_type() const override {
        return queue_type;
    }

    Package pop() override ;

    using iterator = std::list<Package>::iterator;

    const_iterator cbegin() const override { return items.cbegin(); }

    const_iterator cend() const override { return items.cend(); }

    explicit PackageQueue(PackageQueueType queueType) : queue_type(queueType){}

};

#endif //SRC_STORAGE_TYPES_HPP
