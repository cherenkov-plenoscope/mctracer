// Copyright 2014 Sebastian A. Mueller
#include "Core/Numeric.h"
#include <math.h>
using std::vector;

namespace relleums {
namespace numeric {

double mean(const vector<double> &vec) {
    double sum = 0;
    for (double val : vec) sum = sum + val;
    return sum/vec.size();
}

double stddev(const vector<double> &vec) {
    const double m = mean(vec);
    double s = 0.0;
    for (double val : vec) s = s + (val - m)*(val - m);
    return sqrt(s/vec.size());
}

vector<double> linspace(double begin, double end, unsigned int steps) {
    vector<double> lin;
    lin.reserve(steps);
    const double range = end - begin;
    const double step = range/static_cast<double>(steps-1);
    for (unsigned int i = 0; i < steps; i++)
        lin.push_back(begin + step*i);

    return lin;
}

}  // namespace numeric
}  // namespace relleums
