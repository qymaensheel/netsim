//
// Created by bnier on Jan 08 2020.
//

#ifndef INC_09___FACTORY2_NODES_HPP
#define INC_09___FACTORY2_NODES_HPP

#include "package.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include <memory>

enum ReceiverType {
    WORKER, STOREHOUSE
};

class IPackageReceiver {
public:
    virtual void receive_package(Package &&sent_package) = 0;

    virtual ElementID get_id() const = 0;

    virtual ReceiverType get_receiver_type() = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;

    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual IPackageStockpile::const_iterator cend() const = 0;

};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver *, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator prop_gen = default_probability_generator) : prob_generator(prop_gen) {}

    const_iterator preferences_cbegin() const { return preferences.cbegin(); }

    const_iterator preferences_cend() const { return preferences.cend(); }

    const_iterator preferences_begin() const { return preferences.begin(); }

    const_iterator preferences_end() const { return preferences.end(); }

    void add_receiver(IPackageReceiver *receiver_to_add);

    void remove_receiver(IPackageReceiver *receiver_to_remove);

    IPackageReceiver *choose_receiver();

    const preferences_t &get_preferences() const { return preferences; }

    preferences_t preferences;

private:
    ProbabilityGenerator prob_generator;

};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender(ProbabilityGenerator prob_gen = default_probability_generator) : receiver_preferences_(
            prob_gen), ready_to_send_package(std::nullopt) {}

    void send_package();

    std::optional<Package>& get_sending_buffer() { return ready_to_send_package; }



protected:
    void push_package(Package &&package_done) { ready_to_send_package.emplace(std::move(package_done)); }

    std::optional<Package> ready_to_send_package;   //bufor
};


class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset off, std::unique_ptr<IPackageQueue> ipq) : id_(id), off_(off),
                                                                               products_queue(std::move(ipq)) {};

    void do_work(Time t);

    TimeOffset get_processing_duration() { return off_; };

    Time get_package_processing_start_time() { return timeStarted_; };

    IPackageStockpile::const_iterator begin() const override { return products_queue->begin(); };

    IPackageStockpile::const_iterator cbegin() const override { return products_queue->cbegin(); };

    IPackageStockpile::const_iterator end() const override { return products_queue->end(); };

    IPackageStockpile::const_iterator cend() const override { return products_queue->cend(); };

    ElementID get_id() const { return id_; };

    ReceiverType get_receiver_type() { return ReceiverType::WORKER; };

    void receive_package(Package&& p);

private:
    ElementID id_;
    TimeOffset off_;
    std::unique_ptr<IPackageQueue> products_queue;
    std::optional<Package> worked_on_package = std::nullopt;
    unsigned int timeStarted_ = 0;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset off) : id_(id), off_(off) {};

    TimeOffset get_delivery_interval() { return off_; };

    void deliver_goods(Time t);

    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset off_;
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageQueue> ip = std::make_unique<PackageQueue>(PackageQueueType::LIFO)) : id_(id), stored_products(std::move(ip)) {};

    void receive_package(Package &&sent_package) override;

    ElementID get_id() const { return id_; }

    ReceiverType get_receiver_type() override {return ReceiverType::STOREHOUSE;};

    IPackageStockpile::const_iterator begin() const override { return stored_products->begin(); };

    IPackageStockpile::const_iterator cbegin() const override { return stored_products->cbegin(); };

    IPackageStockpile::const_iterator end() const override { return stored_products->end(); };

    IPackageStockpile::const_iterator cend() const override { return stored_products->cend(); };
private:
    ElementID id_;
    std::unique_ptr<IPackageQueue> stored_products;

};

#endif //INC_09___FACTORY2_NODES_HPP
