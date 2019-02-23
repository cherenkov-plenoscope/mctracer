// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_NUMERIC_H_
#define CORE_NUMERIC_H_

#include <vector>

namespace relleums {
namespace numeric {

double mean(const std::vector<double> &vec);
double stddev(const std::vector<double> &vec);
std::vector<double> linspace(double begin, double end, unsigned int steps);

}  // namespace numeric
}  // namespace relleums

#endif  // CORE_NUMERIC_H_
