//
// Created by bnier on Jan 19 2020.
//

#include <simulation.hpp>

void simulate(Factory &factory, TimeOffset d, std::function<void(Factory &, Time)> func) {
    for(TimeOffset time=1;time<=d;time++)
    {
        factory.do_deliveries(time);
        factory.do_package_passing();
        factory.do_work(time);
        func(factory,time);
    }
}

