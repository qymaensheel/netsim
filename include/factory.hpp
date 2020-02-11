#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"

#include <list>

enum NodeColor {
    UNVISITED, VISITED, VERIFIED
};


template <class Node>
class NodeCollection{

public:

    NodeCollection()=default;
    using container_t = typename std::list<Node>;                   //do czego służy typename w tym przypadku
    using iterator =  typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() {return NodesVector.begin();}
    iterator end()   {return NodesVector.end();}
    const_iterator cbegin() {return NodesVector.cbegin();}
    const_iterator cend() {return NodesVector.cend();}

    iterator find_by_id(ElementID id_) { auto it = std::find_if(NodesVector.begin(), NodesVector.end(),[id_](const auto& elem){ return (elem.get_id() == id_);  });
        return it; };
    const_iterator find_by_id(ElementID id_) const {auto it = std::find_if(NodesVector.begin(), NodesVector.end(),[id_](const auto& elem){ return (elem.get_id() == id_);});
        return it; };

    void add(Node& node){NodesVector.push_back(std::move(node));}
    void remove_by_id(ElementID id_to_rem);

private:
    container_t NodesVector;
};

class Factory {
public:
    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

    void add_ramp(Ramp &&Node){Ramps.add(Node);};
    void remove_ramp(ElementID id){Ramps.remove_by_id(id);};
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){ return Ramps.find_by_id(id); };
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return Ramps.find_by_id(id); };
    NodeCollection<Ramp>::const_iterator ramp_cbegin(){ return Ramps.cbegin(); };
    NodeCollection<Ramp>::const_iterator ramp_cend(){ return Ramps.cend(); };

    void add_worker(Worker &&Node){ Workers.add(Node); };
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){ return Workers.find_by_id(id); };
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return Workers.find_by_id(id); };
    NodeCollection<Worker>::const_iterator worker_cbegin(){ return Workers.cbegin(); };
    NodeCollection<Worker>::const_iterator worker_cend(){ return Workers.cend(); };

    void add_storehouse(Storehouse &&Node){ Storehouses.add(Node); };
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){ return Storehouses.find_by_id(id); };
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return Storehouses.find_by_id(id); };
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin(){ return Storehouses.cbegin(); };
    NodeCollection<Storehouse>::const_iterator storehouse_cend(){ return Storehouses.cend(); };
private:
    NodeCollection<Ramp> Ramps;
    NodeCollection<Worker> Workers;
    NodeCollection<Storehouse> Storehouses;
};

#endif //NETSIM_FACTORY_HPP