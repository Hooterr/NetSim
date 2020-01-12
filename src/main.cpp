#include <iostream>
#include "nodes.hpp"

using namespace std;
int main() {
    /*
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

    std::cout << ptr2->size() << std::endl;*/

    IPackageQueue* ptr;
    PackageQueue test(PackageQueueType::FIFO);
    ptr = &test;
    Worker w1(1, 100, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Worker w2(3, 100, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Ramp r1(2, 100);
    r1.receiver_preferences_.add_receiver(&w1);
    r1.receiver_preferences_.add_receiver(&w2);
    r1.deliver_goods(100);
    r1.send_package();
    w1.do_work(200);
    return 0;

}
