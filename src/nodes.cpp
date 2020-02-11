//
// Created by bnier on Jan 08 2020.
//

#include <iostream>
#include "nodes.hpp"

IPackageReceiver *ReceiverPreferences::choose_receiver() {
//    double drew_value = prob_generator()*(*--preferences.end()).second;
//
//    for (auto ptr = preferences.begin(); ptr != preferences.end(); ptr++){
//        if (drew_value<(*ptr).second){
//            return (*ptr).first;
//        }
//    }
    double drewVal = prob_generator();
    if(drewVal==1)
        return (*--preferences.end()).first;
    double buffer=0;
    for(auto ptr = preferences.begin(); ptr != preferences.end(); ptr++){
        if(drewVal>=buffer && drewVal < buffer+(*ptr).second){
            return (*ptr).first;
        } else {
            buffer += (*ptr).second;
        }
    }

    throw std::logic_error("Invalid preferences of object");
}

void ReceiverPreferences::add_receiver(IPackageReceiver *receiver_to_add) {
    double omega=1;
    double tmp=0.0;
    preferences[receiver_to_add]=omega;
    for (auto &receiver : preferences) {
        preferences[receiver.first]=1.0/preferences.size();
        tmp+=receiver.second;
    }
    if(tmp!=1.0) {
        preferences[receiver_to_add]=preferences[receiver_to_add]+1.0-tmp;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *receiver_to_remove) {
    IPackageReceiver* r=receiver_to_remove;
    double tmp=0.0;
    preferences.erase(r);
    if(!preferences.empty()){
        for (const auto &receiver : preferences) {

            preferences[receiver.first]=1.0/preferences.size();
            tmp+=receiver.second;
            r=receiver.first;
        }
//        for(const auto& receiver: preferences){
//            std::cout<<receiver.first->get_id();
//        }
        if(tmp!=1.0) {
            preferences[r]=preferences[r]+1.0-tmp;
        }}
//        for(const auto& receiver: preferences){
//            std::cout<<receiver.first->get_id();
//        }
}

void Worker::do_work(Time t) {
    if(worked_on_package){
        if(off_==1 || timeStarted_ + off_ - t == 1){
            ready_to_send_package = std::move(worked_on_package);
            worked_on_package = std::nullopt;
        }
    } else if ((*products_queue).size()!=0){
        worked_on_package = (*products_queue).pop();
        timeStarted_ = t;
    }
}

void Worker::receive_package(Package&& p){
    products_queue->push(std::move(p));
}

void Ramp::deliver_goods(Time t) {
    if(off_==1 || t % off_ == 1){
        Package p;
        push_package(std::move(p));
    }
}

void Storehouse::receive_package(Package &&sent_package) {
    (*stored_products).push(std::move(sent_package));
}

void PackageSender::send_package() {
    if(ready_to_send_package){
        receiver_preferences_.choose_receiver()->receive_package(std::move(*ready_to_send_package));
        ready_to_send_package = std::nullopt;
    }
}
