//Maksymilian Lach, 302868
#include "package.hpp"

#include <stdexcept>
#include <iostream>

void insert_if_not_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) == s.end()) {
        s.insert(e);
    }
}

void erase_if_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) != s.end()) {
        s.erase(e);
    }
}

Package::Package() {
    if (freed_ids_.empty()) {
        id_ = (*assigned_ids_.rbegin()) + 1;
    } else {
        id_ = *freed_ids_.begin();
        freed_ids_.erase(id_);
    }
    assigned_ids_.insert(id_);
}

Package::Package(ElementID id) {
    if (assigned_ids_.find(id) != assigned_ids_.end()) {
        throw std::invalid_argument("The ID of " + std::to_string(id) + " is already assigned!");
    }

    id_ = id;
    assigned_ids_.insert(id_);
    erase_if_exists(freed_ids_, id);
}

Package::Package(Package&& other) noexcept {
    id_ = other.id_;
    other.id_ = BLANK_ID;
}

Package& Package::operator=(Package&& other) noexcept {
    if (id_ != BLANK_ID) {
        freed_ids_.insert(id_);
        assigned_ids_.erase(id_);
    }

    id_ = other.id_;
    other.id_ = BLANK_ID;

    return *this;
}


Package::~Package() {
    if (id_ != BLANK_ID) {
        insert_if_not_exists(freed_ids_, id_);
        erase_if_exists(assigned_ids_, id_);
    }
}
