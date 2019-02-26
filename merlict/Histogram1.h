// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_HISTOGRAM1D_H_
#define CORE_HISTOGRAM1D_H_

#include <vector>
#include <string>

namespace merlict {

class Histogram1 {
    std::vector<double> bin_edges;

 public:
    std::vector<unsigned int> bins;
    unsigned int number_of_samples = 0;
    unsigned int underflow_bin = 0;
    unsigned int overflow_bin = 0;
    Histogram1(
        const std::vector<double> &samples,
        const std::vector<double> &_bin_edges);
    double mode()const;
    unsigned int arg_max()const;

 private:
    void init_bins_to_zero();
    void assert_at_least_two_bin_edges()const;
    void fill_in(const double sample);
};

}  // namespace relleums

#endif  // CORE_HISTOGRAM1D_H_
