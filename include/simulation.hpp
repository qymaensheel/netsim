//
// Created by bnier on Jan 19 2020.
//

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

void simulate(Factory  &factory, TimeOffset d,std::function<void(Factory&,Time)> func);

#endif //NETSIM_SIMULATION_HPP
