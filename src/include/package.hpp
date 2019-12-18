#ifndef SRC_PACKAGE_HPP
#define SRC_PACKAGE_HPP

#include <stdlib.h>

class Package{
public:
    Package()
    Package(Package&&){}
    Package operator=(Package&&){}
    ElementID get_id(){return id_}
private:
    ElementID id_;
};
#endif //SRC_PACKAGE_HPP
