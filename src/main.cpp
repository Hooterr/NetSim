//Maksymilian Lach, 302868
#include <iostream>
#include "nodes.hpp"
#include "factory.hpp"
#include <sstream>

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

    /*
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
    
    NodeCollection<Ramp> nc;
    nc.add(r1);
     */
    std::string r1 = "LOADING_RAMP id=1 delivery-interval=3";
    std::string r2 = "LOADING_RAMP id=2 delivery-interval=2";
    std::string w1 = "WORKER id=1 processing-time=2 queue-type=FIFO";
    std::string w2 = "WORKER id=2 processing-time=1 queue-type=LIFO";
    std::string s1 = "STOREHOUSE id=1";
    std::string l1 = "LINK src=ramp-1 dest=worker-1";
    std::string l2 = "LINK src=ramp-2 dest=worker-1";
    std::string l3 = "LINK src=ramp-2 dest=worker-2";
    std::string l4 = "LINK src=worker-1 dest=worker-1";
    std::string l5 = "LINK src=worker-1 dest=worker-2";
    std::string l6 = "LINK src=worker-2 dest=store-1";

    std::set<std::string> input_set = {r1, r2, w1, w2, s1, l1, l2, l3, l4, l5, l6};

    std::vector<std::string> input_lines{
            "; == LOADING RAMPS ==",
            "",
            r1,
            r2,
            "",
            "; == WORKERS ==",
            "",
            w1,
            w2,
            "",
            "; == STOREHOUSES ==",
            "",
            s1,
            "",
            "; == LINKS ==",
            "",
            l1,
            "",
            l2,
            l3,
            "",
            l4,
            l5,
            "",
            l6,
    };

    // ignore empty lines, ignore comments ("; ...")
    // resulting set must be empty

    std::string factoryStructureStr = std::accumulate(input_lines.begin(), input_lines.end(), std::string(""),
                                                      [](std::string accu, std::string line) {
                                                          return accu + line + "\n";
                                                      });

    std::istringstream iss(factoryStructureStr);
    Factory factory;
    try {
        factory = load_factory_structure(iss);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        //FAIL();
    } catch (...) {
        std::cerr << "Unknown error..." << std::endl;
        //FAIL();
    }

    std::ostringstream oss;
    save_factory_structure(factory, std::cout);
    return 0;
}
