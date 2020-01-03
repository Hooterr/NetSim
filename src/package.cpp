#include "package.hpp"
#include <stdexcept>

Package::Package() {
    if (free_ids.empty())
    {
        id_ = (*assigned_ids.rbegin()) + 1;
    }
    else
    {
        id_ = *free_ids.begin();
        free_ids.erase(id_);
    }
    assigned_ids.insert(id_);
}

Package::Package(ElementID id) {
    if (assigned_ids.find(id) != assigned_ids.end())
    {
        // NOT GOOD
        throw std::invalid_argument("ID already assigned");
    }
    id_ = id;
    assigned_ids.insert(id_);

    if (free_ids.find(id_) != free_ids.end())
    {
        free_ids.erase(id_);
    }
}

Package::Package(Package && other) noexcept {
    id_ = other.id_;
    other.id_ = BLANK_ID;
}

Package& Package::operator=(Package && other) noexcept {
    if (id_ != BLANK_ID)
    {
        free_ids.insert(id_);
        assigned_ids.erase(id_);
    }

    id_ = other.id_;
    other.id_ = BLANK_ID;

    return *this;
}

Package::~Package() {
    if (id_ != BLANK_ID)
    {
        if (free_ids.find(id_) != free_ids.end())
            free_ids.insert(id_);
        if (assigned_ids.find(id_) != assigned_ids.end())
            assigned_ids.erase(id_);
    }
}