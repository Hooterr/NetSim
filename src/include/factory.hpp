//
// Created by root on 1/12/2020.
//

#ifndef SRC_FACTORY_HPP
#define SRC_FACTORY_HPP

#include "types.hpp"
#include "nodes.hpp"
#include "storage_types.hpp"

#include <stdlib.h>
#include <vector>
#include <istream>
#include <string>

enum COLOR {
    NOT_VISITED,
    VISITED,
    VERIFIED,
};


enum class ElementType
{
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

template <typename Node>
class NodeCollection {
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    // MUST IMPLEMENT GENERIC CLASS HERE BECAUSE C++ IS TOO STUPID TO READ
    // THE IMPLEMENTATION FROM A SOURCE FILE XDDDDDDDDDDDDDDDDDDDDDDDDDD
    void add(Node& node){
        _container.push_back(std::move(node));
    }
    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != _container.end())
            _container.erase(it);
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id){
        return std::find_if(_container.begin(), _container.end(),
                [id](const Node& x) { return x.get_id() == id; });
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {
        return std::find_if(_container.begin(), _container.end(),
                            [id](const Node &x) { return x.get_id() == id; });
    }

    NodeCollection<Node>::const_iterator cbegin() const { return _container.cbegin(); }
    NodeCollection<Node>::const_iterator cend() const { return _container.cend(); }

    NodeCollection<Node>::const_iterator begin() const { return _container.begin(); }
    NodeCollection<Node>::const_iterator end() const { return _container.end(); }

    NodeCollection<Node>::iterator begin() { return _container.begin(); }
    NodeCollection<Node>::iterator end() { return _container.end(); }

private:
    container_t _container;
};

class Factory{
public:
    void add_ramp(Ramp&& ramp)
    { _ramps.add(ramp); }

    void remove_ramp(ElementID id)
    { _ramps.remove_by_id(id); }

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id)
    { return _ramps.find_by_id(id); }

    NodeCollection<Ramp>::const_iterator  find_ramp_by_id(ElementID id) const
    { return _ramps.find_by_id(id); }

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const
    { return _ramps.cbegin(); }

    NodeCollection<Ramp>::const_iterator ramp_cend() const
    { return _ramps.cend(); }


    void add_worker(Worker&& worker)
    { _workers.add(worker); }

    void remove_worker(ElementID id)
    { remove_receiver(_workers, id); }

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id)
    { return _workers.find_by_id(id); }

    NodeCollection<Worker>::const_iterator  find_worker_by_id(ElementID id) const
    { return _workers.find_by_id(id); }

    NodeCollection<Worker>::const_iterator worker_cbegin() const
    { return _workers.cbegin(); }

    NodeCollection<Worker>::const_iterator worker_cend() const
    { return _workers.cend(); }


    void add_storehouse(Storehouse&& storehouse)
    { _storehouses.add(storehouse); }

    void remove_storehouse(ElementID id)
    { remove_receiver(_storehouses, id); }

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id)
    { return _storehouses.find_by_id(id); }

    NodeCollection<Storehouse>::const_iterator  find_storehouse_by_id(ElementID id) const
    { return _storehouses.find_by_id(id); }

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const
    { return _storehouses.cbegin(); }

    NodeCollection<Storehouse>::const_iterator storehouse_cend() const
    { return _storehouses.cend(); }

    bool is_consistent() const;
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    bool can_reach_storehouse(const PackageSender* sender, std::map<const PackageSender*, COLOR> &colors) const;
    NodeCollection<Ramp> _ramps;
    NodeCollection<Worker> _workers;
    NodeCollection<Storehouse> _storehouses;

    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id)
    {
        typename NodeCollection<Node>::iterator item = collection.find_by_id(id);
        if (item == collection.end())
            return;

        std::for_each(_ramps.begin(), _ramps.end(), [item](Ramp& ramp)
        {
           ramp.receiver_preferences_.remove_receiver(
                   dynamic_cast<IPackageReceiver*>(&(*item)));
        });

        std::for_each(_workers.begin(), _workers.end(), [item](Worker& worker)
        {
            worker.receiver_preferences_.remove_receiver(
                    dynamic_cast<IPackageReceiver*>(&(*item)));
        });

        collection.remove_by_id(id);
    }
};

Factory load_factory_structure(std::istream &is);
void save_factory_structure(Factory& factory, std::ostream &os);


#endif //SRC_FACTORY_HPP
