// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "plenoscope/calibration/OnlineStatistics.h"
#include "merlict/random/random.h"
#include "merlict/numeric.h"
using std::vector;
using namespace plenoscope;
using namespace merlict;



TEST_CASE("OnlineStatisticsTest: few_samples", "[merlict]") {
    vector<double> vals = {1.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    CHECK(1.0 == ov.mean());
    CHECK(1 == ov.number_of_samples());
    CHECK(ov.variance() != ov.variance());
    CHECK(ov.stddev() != ov.stddev());
}

TEST_CASE("OnlineStatisticsTest: online_variance", "[merlict]") {
    vector<double> vals;
    random::Mt19937 prng(0);
    OnlineStatistics ov;
    for (unsigned int i = 0; i < 1000000; i++) {
        const double r = prng.uniform();
        vals.push_back(r);
        ov.add(r);
    }
    double stddev_using_online = ov.stddev();
    double stddev_using_classic = numeric::stddev(vals);
    CHECK(stddev_using_online == Approx(stddev_using_classic).margin(1e-6));
    CHECK(1000000 == ov.number_of_samples());
}

TEST_CASE("OnlineStatisticsTest: online_variance_simple_numbers", "[merlict]") {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    double stddev_using_online = ov.stddev();
    double stddev_using_classic = numeric::stddev(vals);
    CHECK(stddev_using_online == Approx(stddev_using_classic).margin(1e-6));
    CHECK(7 == ov.number_of_samples());
}

TEST_CASE("OnlineStatisticsTest: sum", "[merlict]") {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    CHECK(21.0 == ov.sum());
}

TEST_CASE("OnlineStatisticsTest: mean", "[merlict]") {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    CHECK(3.0 == ov.mean());
}
