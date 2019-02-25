// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/Histogram1.h"
#include "Core/random/random.h"
#include "Core/tools.h"
#include "Core/numeric.h"
using std::vector;
using namespace relleums;



TEST_CASE("Histogram1Test: empty_bin_edges", "[mctracer]") {
    vector<double> bins_edges;
    random::Mt19937 prng(0);
    vector<double> samples;
    for (unsigned int i = 0; i < 42*1337; i++)
        samples.push_back(prng.uniform());
    CHECK_THROWS_AS(Histogram1 histo(samples, bins_edges), std::invalid_argument);
}

TEST_CASE("Histogram1Test: init", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    random::Mt19937 prng(0);
    vector<double> samples;
    for (unsigned int i = 0; i < 42*1337; i++)
        samples.push_back(prng.uniform());
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
}

TEST_CASE("Histogram1Test: underflow_bin_above_expect_empty", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
}

TEST_CASE("Histogram1Test: underflow_bin_exact_expect_empty", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.0, 0.1, 0.2, 0.3};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
}

TEST_CASE("Histogram1Test: underflow_bin_below_expect_full", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {-1e-9, 0.1, 0.2, 0.3};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 1u);
    CHECK(histo.overflow_bin == 0u);
}

TEST_CASE("Histogram1Test: overflow_bin_above_expect_full", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0+1e-9};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 1u);
}

TEST_CASE("Histogram1Test: overflow_bin_exact_expect_full", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 1u);
}

TEST_CASE("Histogram1Test: overflow_bin_below_expect_empty", "[mctracer]") {
    vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0-1e-9};
    Histogram1 histo(samples, bins);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
}

TEST_CASE("Histogram1Test: all_in_one_bin_middle", "[mctracer]") {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.5);
    Histogram1 histo(samples, bins_edges);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
    REQUIRE(histo.bins.size() == 3u);
    CHECK(histo.bins[0] == 0u);
    CHECK(histo.bins[1] == 42u*1337u);
    CHECK(histo.bins[2] == 0u);
}

TEST_CASE("Histogram1Test: all_in_one_bin_front", "[mctracer]") {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.15);
    Histogram1 histo(samples, bins_edges);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
    REQUIRE(histo.bins.size() == 3u);
    CHECK(histo.bins[0] == 42u*1337u);
    CHECK(histo.bins[1] == 0u);
    CHECK(histo.bins[2] == 0u);
}

TEST_CASE("Histogram1Test: all_in_one_bin_back", "[mctracer]") {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.75);
    Histogram1 histo(samples, bins_edges);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
    REQUIRE(histo.bins.size() == 3u);
    CHECK(histo.bins[0] == 0u);
    CHECK(histo.bins[1] == 0u);
    CHECK(histo.bins[2] == 42u*1337u);
}

TEST_CASE("Histogram1Test: arg_max", "[mctracer]") {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    //             2       6           1
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.9};
    Histogram1 histo(samples, bins_edges);
    CHECK(histo.underflow_bin == 0u);
    CHECK(histo.overflow_bin == 0u);
    REQUIRE(histo.bins.size() == 3u);
    CHECK(histo.bins[0] == 2u);
    CHECK(histo.bins[1] == 5u);
    CHECK(histo.bins[2] == 1u);
    CHECK(histo.arg_max() == 1u);
}

TEST_CASE("Histogram1Test: arg_max_empty", "[mctracer]") {
    vector<double> bins_edges;
    vector<double> samples;
    CHECK_THROWS_AS(Histogram1 histo(samples, bins_edges), std::invalid_argument);
}

TEST_CASE("Histogram1Test: arg_max_empty_sample", "[mctracer]") {
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    Histogram1 histo(samples, bins_edges);
    CHECK(histo.arg_max() == 0u);
}

TEST_CASE("Histogram1Test: arg_max_empty_edges", "[mctracer]") {
    vector<double> bins_edges;
    vector<double> samples = {0.1, 0.2, 0.5};
    CHECK_THROWS_AS(Histogram1 histo(samples, bins_edges), std::invalid_argument);
}

TEST_CASE("Histogram1Test: mode_empty_sample", "[mctracer]") {
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    Histogram1 histo(samples, bins_edges);
    CHECK(1./6. == Approx(histo.mode()).margin(1e-6));
}

TEST_CASE("Histogram1Test: mode_front", "[mctracer]") {
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.1, 0.1, 0.2, 0.5};
    Histogram1 histo(samples, bins_edges);
    CHECK(1./6. == Approx(histo.mode()).margin(1e-6));
}

TEST_CASE("Histogram1Test: mode_middle", "[mctracer]") {
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.5, 0.5, 0.5, 0.7, 0.7};
    Histogram1 histo(samples, bins_edges);
    CHECK(0.5 == Approx(histo.mode()).margin(1e-6));
}

TEST_CASE("Histogram1Test: mode_back", "[mctracer]") {
    vector<double> bins_edges = numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.5, 0.7, 0.7};
    Histogram1 histo(samples, bins_edges);
    CHECK(5./6. == Approx(histo.mode()).margin(1e-6));
}
