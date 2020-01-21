//
// Created by lsriw on 1/14/20.
//

#include "reports.hpp"
#include <sstream>
#include <map>

void generate_structure_report(const Factory &factory, std::ostream &os){

    os << "\n== LOADING RAMPS ==\n\n";
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp){
        os << "LOADING RAMP #" << ramp.get_id() << "\n";
        os << "  Delivery interval: " << ramp.get_delivery_interval() << "\n";
        os << "  Receivers:" << "\n";
        auto prefs = ramp.receiver_preferences_.get_preferences();
        std::for_each(prefs.cbegin(), prefs.cend(), [&os](const std::pair<IPackageReceiver*,double> pair) {
            os << "    worker #" << pair.first->get_id()<< "\n";
        });
        os << "\n";
    });

    std::map<ElementID, std::string> workers, stores;

    os << "\n== WORKERS ==\n\n";
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os, &workers, &stores](const Worker& worker) {
        os << "WORKER #" << worker.get_id() << "\n";
        os << "  Processing time: " << worker.get_processing_duration() << "\n";
        // TODO enum to string or something..
        os << "  Queue type: " << (worker.get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO") << "\n";
        os << "  Receivers:\n";

        auto prefs = worker.receiver_preferences_.get_preferences();
        std::for_each(prefs.cbegin(), prefs.cend(), [&workers, &stores](const std::pair<IPackageReceiver*,double> pair) {
            std::stringstream ss;
            ss << "    " << (pair.first->get_receiver_type() == ReceiverType::WORKER ? "worker" : "storehouse");
            ss << " #" << pair.first->get_id()<< "\n";

            if (pair.first->get_receiver_type() == ReceiverType::WORKER)
                workers.insert(std::pair<ElementID , std::string>(pair.first->get_id(), ss.str()));
            else
                stores.insert(std::pair<ElementID , std::string>(pair.first->get_id(), ss.str()));
        });

        std::for_each(stores.cbegin(), stores.cend(), [&os](const auto &pair) { os << pair.second; });
        std::for_each(workers.cbegin(), workers.cend(), [&os](const auto &pair) { os << pair.second; });

        stores.clear();
        workers.clear();

        os << "\n";

    });

    std::map<ElementID, std::string> strs;
    os << "\n== STOREHOUSES ==\n";
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&strs](const Storehouse& storehouse) {
        std::stringstream ss;
        ss << "\nSTOREHOUSE #" << storehouse.get_id() << "\n";
        strs.insert(std::pair<ElementID, std::string>(storehouse.get_id(), ss.str()));
    });

    std::for_each(strs.cbegin(), strs.cend(), [&os](const auto &pair) { os << pair.second; });

    os << "\n";
}

void generate_simulation_turn_report(const Factory &factory, std::ostream &os, Time t)
{
    os << "=== [ Turn: " << std::to_string(t) <<" ] ===\n";
    os << "\n== WORKERS ==\n\n";
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++) {
        const Worker& worker = *it;
        os << "WORKER #" << worker.get_id() << "\n";
        os << "  PBuffer: ";
        if(worker.get_processing_buffer())
            os << "#" + std::to_string(worker.get_processing_buffer().value().get_id())
               <<(" (pt = ") << std::to_string((int)worker.get_time_spent(t)) << ")";
        else
            os << "(empty)";

        os << "\n  Queue: ";

        auto queue = worker.get_queue();

        if (queue->cbegin() == queue->cend())
            os << "(empty)";

        for (auto package = queue->cbegin(); package != queue->cend(); package++)
        {
            os << "#" << package->get_id();
            if (worker.get_processing_buffer() && &(worker.get_processing_buffer().value()) == &(*package))
                os << " (pt = " << worker.get_time_spent(t) << ")";

           os << (package == std::prev(queue->cend()) ? "" : ",");
        }

        os << "\n";

        os << "  SBuffer: " << (worker.get_sending_buffer() ? "#" + std::to_string(worker.get_sending_buffer().value().get_id()) : "(empty)")
           << "\n\n";
    }

    os << "\n== STOREHOUSES ==\n\n";

    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); it++) {
        const Storehouse& store = *it;
        os << "STOREHOUSE #" << store.get_id() << "\n";
        os << "  Stock: ";

        if (store.cbegin() == store.cend())
            os << "(empty)";

        for (auto package = store.cbegin(); package != store.cend(); package++)
        {
            os << "#" << package->get_id();
            os << (package == std::prev(store.cend()) ? "" : ",");
        }

        os << "\n\n";
    }
}
