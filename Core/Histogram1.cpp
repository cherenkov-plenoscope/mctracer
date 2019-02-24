// Copyright 2014 Sebastian A. Mueller
#include <algorithm>
#include <sstream>
#include "Core/Histogram1.h"

using std::string;
using std::vector;

namespace relleums {

Histogram1::Histogram1(
    const vector<double> &samples,
    const vector<double> &_bin_edges
):bin_edges(_bin_edges) {
    assert_at_least_two_bin_edges();
    std::sort(bin_edges.begin(), bin_edges.end());
    init_bins_to_zero();
    for (double sample : samples)
        fill_in(sample);
    number_of_samples = samples.size();
}

void Histogram1::init_bins_to_zero() {
    unsigned int bin_count = bin_edges.size() - 1;
    bins.reserve(bin_count);
    for (unsigned int i = 0; i < bin_count; i++)
        bins.push_back(0);
}

void Histogram1::fill_in(const double sample) {
    vector<double>::const_iterator up_bin_edge = std::upper_bound(
        bin_edges.begin(),
        bin_edges.end(),
        sample);

    if (up_bin_edge == bin_edges.begin()) {
        underflow_bin++;
        return;
    }

    if (up_bin_edge == bin_edges.end()) {
        overflow_bin++;
        return;
    }

    unsigned int bin_index = (up_bin_edge - bin_edges.begin()) - 1;
    bins.at(bin_index)++;
}

double Histogram1::mode()const {
    unsigned int idx_max_bin = arg_max();
    return 0.5*(bin_edges.at(idx_max_bin+1) + bin_edges.at(idx_max_bin));
}

unsigned int Histogram1::arg_max()const {
    unsigned int idx_max_bin = 0;
    unsigned int max_bin_count = 0;

    for (unsigned int i = 0; i < bins.size(); i++)
        if (bins.at(i) > max_bin_count) {
            idx_max_bin = i;
            max_bin_count = bins.at(i);
        }

    return idx_max_bin;
}

void Histogram1::assert_at_least_two_bin_edges()const {
    if (bin_edges.size() < 2) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << ", " << __func__ << "\n";
        info << "Expected at least two bin edges, but actual there is only ";
        info << bin_edges.size() << ".\n";
        throw std::invalid_argument(info.str());
    }
}

}  // namespace relleums
