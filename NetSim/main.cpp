//Maksymilian Lach, 302868
#include <iostream>
#include "nodes.hpp"
#include "factory.hpp"
#include "reports.hpp"
#include <sstream>


void test ()
{
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
    factory.do_deliveries(1);
    factory.do_package_passing();
    factory.do_work(1);
    generate_simulation_turn_report(factory, std::cout, 1);

    factory.do_deliveries(2);
    factory.do_package_passing();
    factory.do_work(2);
    generate_simulation_turn_report(factory, std::cout, 2);

    factory.do_deliveries(3);
    factory.do_package_passing();
    factory.do_work(3);
    generate_simulation_turn_report(factory, std::cout, 3);

    factory.do_deliveries(4);
    factory.do_package_passing();
    factory.do_work(4);
    generate_simulation_turn_report(factory, std::cout, 4);

    factory.do_deliveries(5);
    factory.do_package_passing();
    factory.do_work(5);
    //generate_simulation_turn_report(factory, std::cout, 5);

    factory.do_deliveries(6);
    factory.do_package_passing();
    factory.do_work(6);
    generate_simulation_turn_report(factory, std::cout, 6);


}
using namespace std;
int main() {

    test();


    return 0;
}
