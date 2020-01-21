//Maksymilian Lach, 302868
#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <set>

#include "types.hpp"

class Package {
public:
    Package();
    explicit Package(ElementID id);
    Package(const Package&) = delete;
    Package(Package&&) noexcept;
    ~Package();
    ElementID get_id() const { return id_; }

    Package& operator=(Package&) = delete;
    Package& operator=(Package&&) noexcept;

private:
    static const ElementID BLANK_ID = -1;
    inline static std::set<ElementID> assigned_ids_{0};
    inline static std::set<ElementID> freed_ids_;
    ElementID id_ = BLANK_ID;
};

#endif //PACKAGE_HPP
