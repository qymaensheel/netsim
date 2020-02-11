//
// Created by bnier on Jan 08 2020.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    //jesli lista pusta to rzuc wyjatek wewnatrz funkcji
    if(queue.begin() != queue.end()){
        Package p;
        switch(queue_type){
            case PackageQueueType::FIFO:
                p = std::move(queue.front());
                queue.pop_front();
                return p;
            case PackageQueueType::LIFO:
                p = std::move(queue.back());
                queue.pop_back();
                return p;
            default:
                p = std::move(queue.front());
                queue.pop_front();
                return p;
        }
    }
    else {
        throw std::logic_error("Queue must not be empty!");
    }
}

void PackageQueue::push(Package&& pack) {
    switch(queue_type){
        case PackageQueueType::FIFO:
            queue.emplace_back(std::move(pack));
            break;
        case PackageQueueType::LIFO:
            queue.emplace_back(std::move(pack));
            break;
    }
}
