// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_NUMERIC_H_
#define MERLICT_NUMERIC_H_

#include <vector>

namespace merlict {
namespace numeric {

double mean(const std::vector<double> &vec);
double stddev(const std::vector<double> &vec);
std::vector<double> linspace(double begin, double end, unsigned int steps);

}  // namespace numeric
}  // namespace merlict

#endif  // MERLICT_NUMERIC_H_
