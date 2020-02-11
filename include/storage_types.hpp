//
// Created by bnier on Jan 08 2020.
//

#ifndef INC_09___FACTORY2_STORAGE_TYPES_HPP
#define INC_09___FACTORY2_STORAGE_TYPES_HPP

#include <list>
#include "package.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include <optional>

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package &&) = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
    virtual std::size_t  size() const = 0;
    virtual bool empty() const = 0;

};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType type): queue(std::list<Package>()), queue_type(type) {};
    void push(Package &&) override;
    bool empty() const override { return queue.empty(); };
    std::size_t size() const override { return queue.size(); };
    Package pop() override;
    PackageQueueType get_queue_type() const override { return queue_type; };

    const_iterator cbegin() const override { return queue.cbegin(); }
    const_iterator cend() const override { return queue.cend(); }
    const_iterator begin() const override { return queue.cbegin(); }
    const_iterator end() const override { return queue.cend(); }


private:
    std::list<Package> queue;
    PackageQueueType queue_type;
};


#endif //INC_09___FACTORY2_STORAGE_TYPES_HPP
