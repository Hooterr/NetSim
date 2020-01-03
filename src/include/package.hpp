#ifndef SRC_PACKAGE_HPP
#define SRC_PACKAGE_HPP

#include <set>
#include <stdexcept>
#include "types.hpp"

class Package{
public:
    Package();
    explicit Package(ElementID id);

    Package(const Package&) = delete;
    Package(Package&&) noexcept;

    Package& operator=(Package&) = delete;
    Package& operator=(Package&&) noexcept;

    ~Package();

    ElementID get_id() const { return id_; }

private:
    static const ElementID BLANK_ID = -1;

    inline static std::set<ElementID> assigned_ids{0};
    inline static std::set<ElementID> free_ids{0};

    ElementID id_ = BLANK_ID;
};

#endif //SRC_PACKAGE_HPP