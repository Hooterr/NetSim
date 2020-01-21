//
// Created by root on 1/12/2020.
//
#include "factory.hpp"
#include "nodes.hpp"
#include <algorithm>
#include <map>
#include <exception>
#include <map>
#include <sstream>

bool Factory::is_consistent() const {

    auto colors = std::map<const PackageSender*, COLOR>();
    for(auto it = _ramps.cbegin(); it != _ramps.cend(); it++) {
        colors[dynamic_cast<const PackageSender*>(&(*it))] = COLOR::NOT_VISITED;
    }

    for(auto it = _workers.cbegin(); it != _workers.cend(); it++) {
        colors[dynamic_cast<const PackageSender*>(&(*it))] = COLOR::NOT_VISITED;
    }

    for(auto it = _ramps.cbegin(); it != _ramps.cend(); it++) {
        try{
            can_reach_storehouse(dynamic_cast<const PackageSender*>(&(*it)), colors);
        }
        catch (const std::logic_error&)
        {
            return false;
        }
    }

    return true;
}

bool Factory::can_reach_storehouse(const PackageSender* sender, std::map<const PackageSender*, COLOR> &colors) const
{
    if (colors[sender] == COLOR::VERIFIED)
        return true;

    colors[sender] = COLOR::VISITED;

    const std::map<IPackageReceiver*, double>& prefs =
            sender->receiver_preferences_.get_preferences();

    if(prefs.size() == 0)
        throw std::logic_error("");

    bool has_receiver_other_than_self = false;
    for(auto pair : prefs)
    {
        if (pair.first->get_receiver_type() == ReceiverType::STOREHOUSE)
        {
            has_receiver_other_than_self = true;
        }
        else
        {
            auto worker_ptr = dynamic_cast<Worker*>(pair.first);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (worker_ptr == sender)
                continue;

            has_receiver_other_than_self = true;

            if(colors[sendrecv_ptr] == COLOR::NOT_VISITED)
                can_reach_storehouse(sendrecv_ptr, colors);
        }
    }
    colors[sender] = COLOR::VERIFIED;

    if(has_receiver_other_than_self)
        return true;

    throw std::logic_error("");
}

void Factory::do_deliveries(Time t) {
    std::for_each(_ramps.begin(), _ramps.end(), [t](Ramp& x) { x.deliver_goods(t);});
}

void Factory::do_package_passing() {
    std::for_each(_ramps.begin(), _ramps.end(), [](Ramp& x) { x.send_package();});
    std::for_each(_workers.begin(), _workers.end(), [](Worker& x) { x.send_package();});
}

void Factory::do_work(Time t) {
    std::for_each(_workers.begin(), _workers.end(), [t](Worker& x) { x.do_work(t);});
}


struct ParsedLine
{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLine parse_line(std::string line)
{
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    if(tokens.empty())
        throw std::logic_error("The line does not contain any valid tokens");

    ParsedLine pl;

    if(tokens[0] == "LOADING_RAMP")
        pl.element_type = ElementType::RAMP;
    else if(tokens[0] == "WORKER")
        pl.element_type = ElementType::WORKER;
    else if(tokens[0] == "STOREHOUSE")
        pl.element_type = ElementType::STOREHOUSE;
    else if(tokens[0] == "LINK")
        pl.element_type = ElementType::LINK;
    else
        throw new std::logic_error("Unknown tag");

    for (auto it = tokens.begin() + 1; it != tokens.end(); it++)
    {
        auto delim_pos = it->find("=");
        if (delim_pos == std::string::npos)
            throw std::logic_error("Invalid key value pair.");

        std::string key = it->substr(0, delim_pos);
        std::string value = it->substr(delim_pos + 1);
        pl.parameters[key] = value;
    }

    return pl;
}

void parse_link_endpoint(std::string line, std::string* dstType, ElementID* id)
{
    auto delimPost = line.find('-');
    if(delimPost == std::string::npos)
        throw std::logic_error("Link endpoint is not valid");

    std::string type = line.substr(0, delimPost);
    std::string id_str = line.substr(delimPost + 1);
    *id = std::stoi(id_str);
    *dstType = type;
}

Factory load_factory_structure(std::istream &is)
{
    Factory factory;

    for(std::string line; std::getline(is, line);) {
        if(line.empty() || line[0] == ';')
            continue;
        ParsedLine pl = parse_line(line);

        switch(pl.element_type)
        {
            case ElementType::RAMP: {
                ElementID id = std::stoi(pl.parameters["id"]);
                TimeOffset di = std::stod(pl.parameters["delivery-interval"]);
                factory.add_ramp(Ramp(id, di));
                break;
            }
            case ElementType::WORKER: {
                ElementID id = std::stoi(pl.parameters["id"]);
                TimeOffset pt = std::stod(pl.parameters["processing-time"]);
                PackageQueueType qt;
                std::string queue_type_str = pl.parameters["queue-type"];
                if(queue_type_str == "FIFO")
                    qt = PackageQueueType::FIFO;
                else if (queue_type_str == "LIFO")
                    qt = PackageQueueType::LIFO;
                else
                    throw std::logic_error("Unknown queue-type");
                factory.add_worker(Worker(id, pt,
                        std::make_unique<PackageQueue>(qt)));
                break;
            }
            case ElementType::STOREHOUSE: {
                ElementID id = std::stoi(pl.parameters["id"]);
                factory.add_storehouse(Storehouse(id));
                break;
            }
            case ElementType::LINK: {
                std::string source = pl.parameters["src"];
                std::string sourceType;
                ElementID sourceId;
                parse_link_endpoint(source, &sourceType, &sourceId);

                std::string destination = pl.parameters["dest"];
                std::string destinationType;
                ElementID destinationId;
                parse_link_endpoint(destination, &destinationType, &destinationId);

                IPackageReceiver* dest_receiver;

                if (destinationType == "worker") {
                    auto worker = factory.find_worker_by_id(destinationId);
                    if (worker == factory.worker_cend())
                        throw std::logic_error("Link's destination element does not exist");
                    dest_receiver = dynamic_cast<IPackageReceiver*>(&(*worker));
                }
                else if (destinationType == "store") {
                    auto store = factory.find_storehouse_by_id(destinationId);
                    if (store == factory.storehouse_cend())
                        throw std::logic_error("Link's destination element does not exist");
                    dest_receiver = dynamic_cast<IPackageReceiver*>(&(*store));
                }
                else
                    throw std::logic_error("Destination type name not recognized.");

                PackageSender* sender;
                if (sourceType == "ramp") {
                    auto ramp = factory.find_ramp_by_id(sourceId);
                    if (ramp == factory.ramp_cend())
                        throw std::logic_error("Link's source element does not exist");

                    sender = dynamic_cast<PackageSender*>(&(*ramp));
                }
                else if (sourceType == "worker") {
                    auto worker = factory.find_worker_by_id(sourceId);
                    if (worker == factory.worker_cend())
                        throw std::logic_error("Link's source element does not exist");

                    sender = dynamic_cast<PackageSender*>(&(*worker));
                }
                else
                    throw std::logic_error("Source type name not recognized.");

                sender->receiver_preferences_.add_receiver(dest_receiver);

                break;
            }
            default:
                throw std::logic_error("Unknown ElementType");
        }
    }

    return factory;
}


void save_factory_structure(Factory& factory, std::ostream &os)
{
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp){
        os << "LOADING_RAMP id=" << ramp.get_id()
           << " delivery-interval=" << ramp.get_delivery_interval() << "\n";
    });

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker) {
        os << "WORKER id=" << worker.get_id()
           << " processing-time=" << worker.get_processing_duration()
           << " queue-type=" << (worker.get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO")
           << "\n";

    });

    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse) {
        os << "STOREHOUSE id=" << storehouse.get_id() << "\n";
    });

    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp) {
        auto prefs = ramp.receiver_preferences_.get_preferences();
        std::for_each(prefs.cbegin(), prefs.cend(), [&os, &ramp](const std::pair<IPackageReceiver*,double> pair) {
            os << "LINK src=ramp-" << ramp.get_id()
               << " dest=" << (pair.first->get_receiver_type() == ReceiverType::WORKER ? "worker" : "store")
               << "-" << pair.first->get_id() << "\n";
        });
    });

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker) {
        auto prefs = worker.receiver_preferences_.get_preferences();
        std::for_each(prefs.cbegin(), prefs.cend(), [&os, &worker](const std::pair<IPackageReceiver*,double> pair) {
            os << "LINK src=worker-" << worker.get_id()
               << " dest=" << (pair.first->get_receiver_type() == ReceiverType::WORKER ? "worker" : "store")
               << "-" << pair.first->get_id() << "\n";
        });
    });
}



