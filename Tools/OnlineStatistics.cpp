// Copyright 2014 Sebastian A. Mueller#
#include "Tools/OnlineStatistics.h"
#include <math.h>
#include <limits>

namespace relleums {

OnlineStatistics::OnlineStatistics():
    n_samples(0.0),
    _sum(0.0),
    smean(0.0),
    m2(0.0)
{}

void OnlineStatistics::add(const double x) {
    // Online variance
    n_samples += 1.0;
    const double delta = x - smean;
    smean += delta/n_samples;
    m2 += delta*(x - smean);

    // Online Mean
    _sum += x;
}

double OnlineStatistics::variance()const {
    if (n_samples < 2.0)
        return std::numeric_limits<double>::quiet_NaN();
    else
        return m2/n_samples;

    // In wikipedia it is: m2/(number_of_samples - 1.0).
    // We remove the substraction of 1.0, since it does not fit to the classic
    // stddev algorithms.
}

double OnlineStatistics::stddev()const {
    return sqrt(variance());
}

double OnlineStatistics::mean()const {
    return _sum/n_samples;
}

double OnlineStatistics::sum()const {
    return _sum;
}

double OnlineStatistics::number_of_samples()const {
    return n_samples;
}

}  // namespace relleums
