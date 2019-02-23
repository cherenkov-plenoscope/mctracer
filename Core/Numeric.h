// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_NUMERIC_H_
#define TOOLS_NUMERIC_H_

#include <vector>

namespace relleums {
namespace Numeric {

double mean(const std::vector<double> &vec);
double stddev(const std::vector<double> &vec);
std::vector<double> linspace(double begin, double end, unsigned int steps);

}
}  // namespace relleums

#endif  // TOOLS_NUMERIC_H_
