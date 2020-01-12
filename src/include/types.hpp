//
// Created by lsriw on 12/17/19.
//

#ifndef SRC_TYPES_HPP
#define SRC_TYPES_HPP

#include <functional>

using ElementID = int;

using Time = double;
using TimeOffset = double;
using ProbabilityGenerator = std::function<double(void)>;

#endif //SRC_TYPES_HPP
