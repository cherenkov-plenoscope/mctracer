// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_ONLINESTATISTICS_H_
#define TOOLS_ONLINESTATISTICS_H_

namespace plenoscope {

class OnlineStatistics {
    // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
    // Online algorithm
    double n_samples;
    double _sum;
    double smean;
    double m2;

 public:
    OnlineStatistics();
    void add(const double x);
    double variance()const;
    double stddev()const;
    double mean()const;
    double sum()const;
    double num_samples()const;
};

}  // namespace plenoscope

#endif  // TOOLS_ONLINESTATISTICS_H_
