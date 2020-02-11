//
// Created by bnier on Jan 08 2020.
//

#ifndef INC_09___FACTORY2_TYPES_HPP
#define INC_09___FACTORY2_TYPES_HPP

#include <functional>
#include <map>



using ElementID = unsigned int;
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double()>;
enum class ObjectType {
    storage, ramp, worker
};
#endif //INC_09___FACTORY2_TYPES_HPP
