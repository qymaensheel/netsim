#include "factory.hpp"

#include <iostream>



template<class Node> void NodeCollection<Node>::remove_by_id(ElementID id_to_rem) {
    auto it_to_rem=find_by_id(id_to_rem);
    if(it_to_rem!=NodesVector.end()){
        NodesVector.erase(it_to_rem);
    }
}

void Factory::do_work(Time t) {
    for(auto& worker: Workers)
        worker.do_work(t);
}

void Factory::do_deliveries(Time t) {
    for(auto& ramp: Ramps)
        ramp.deliver_goods(t);
}

void Factory::do_package_passing() {
    for(auto& worker: Workers)
        worker.send_package();
    for(auto& ramp: Ramps)
        ramp.send_package();
}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if(node_colors[sender] == NodeColor::VERIFIED)
    {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if(sender->receiver_preferences_.preferences.empty())
    {
        throw std::logic_error("Error - network is inconsistent");
    }

    bool isNotEmpty = false;
    for(auto& receiver: sender->receiver_preferences_.preferences)
    {
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
        {
            isNotEmpty = true;
        } else
        {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrecv_ptr != sender) isNotEmpty = true;
            if(node_colors[sendrecv_ptr] != NodeColor::VISITED)
            {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(isNotEmpty) return true;
    else throw std::logic_error("Error - network is inconsistent");
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> nodeColor;
    for(auto& worker: Workers) nodeColor.emplace(std::make_pair(&worker, NodeColor::UNVISITED));
    for(auto& ramp: Ramps) nodeColor.emplace(std::make_pair(&ramp, NodeColor::UNVISITED));

    for(const auto& sender: Ramps)
    {
        try
        {
            has_reachable_storehouse(&sender, nodeColor);
        }
        catch(const std::logic_error& )
        {
            return false;
        }
    }
    return true;
}

void Factory::remove_worker(ElementID id) {

    std::for_each(Ramps.begin(),Ramps.end(),[id](Ramp& ramp) {
        auto p=ramp.receiver_preferences_.get_preferences();

        for(auto r=p.begin(); r!=p.end();r++) {
            if(r->first->get_id()==id) {
                ramp.receiver_preferences_.remove_receiver(r->first);
            }
        }
    } );

    Workers.remove_by_id(id);

}

void Factory::remove_storehouse(ElementID id) {
    std::for_each(Workers.begin(),Workers.end(),[id](Worker& worker) {
        auto p=worker.receiver_preferences_.get_preferences();
        for(auto r=p.begin(); r!=p.end();r++) {
            if(r->first->get_id()==id) {
                worker.receiver_preferences_.remove_receiver(r->first);
            }
        }
    } );

    Storehouses.remove_by_id(id);

}
