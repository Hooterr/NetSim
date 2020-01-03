#include <iostream>
#include "include/storage_types.hpp"



int main() {
    IPackageQueue* ptr;
    IPackageStockpile* ptr2;
    PackageQueue test(PackageQueueType::FIFO);
    ptr = &test;
    ptr2 = &test;
    std::cout << (ptr->get_queue_type() == PackageQueueType::FIFO ? "ok" : "fuck") << std::endl;

    std::cout << "Hello, World!" << std::endl;

    Package *p1;
    Package pp1;
    p1 = & pp1;
    std::cout << p1->get_id() << std::endl;

    ptr->push(std::move(pp1));
    std::cout << "queue size:" << ptr2->size() << std::endl;
    std::cout << (ptr->empty() == 0 ? "not empty" : "empty") << std::endl;
    Package& pppp = ptr->pop();
    std::cout << p1->get_id() << std::endl;
    std::cout << pp1.get_id() << std::endl;
    std::cout << pppp.get_id() << std::endl;

    std::cout << ptr2->size() << std::endl;
    return 0;

}
