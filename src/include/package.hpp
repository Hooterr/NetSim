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

    Package& operator=(Package&) = delete;
    Package& operator=(Package&&) noexcept;

    ~Package();

    ElementID get_id() const { return id_; }

private:
    static const ElementID BLANK_ID = -1;

    /*
     * Począwszy od C++17 pola statyczne mogą być oznaczane jako `inline`,
     * dzięki czemu moża je inicjalizować wewnątrz definicji klasy (a nie
     * w osobnym pliku źródłowym).
     */

    // Pula identyfikatorów aktualnie przyporządkowanych półproduktom.
    inline static std::set<ElementID> assigned_ids_{0};

    // Pula identyfikatorów, które zostały zwolnione w wyniku usuwania półproduktów.
    inline static std::set<ElementID> freed_ids_;

    ElementID id_ = BLANK_ID;
};

#endif //PACKAGE_HPP
