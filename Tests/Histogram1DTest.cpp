// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/Histogram1D.h"
#include "Core/Random/Random.h"
#include "Tools/Tools.h"
#include "Tools/Numeric.h"
using std::vector;
using namespace relleums;

class Histogram1DTest : public ::testing::Test {};

TEST_F(Histogram1DTest, empty_bin_edges) {
    vector<double> bins_edges;
    random::Mt19937 prng(0);
    vector<double> samples;
    for (unsigned int i = 0; i < 42*1337; i++)
        samples.push_back(prng.uniform());
    EXPECT_THROW(
        Histogram1D histo(samples, bins_edges),
        std::invalid_argument);
}

TEST_F(Histogram1DTest, init) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    random::Mt19937 prng(0);
    vector<double> samples;
    for (unsigned int i = 0; i < 42*1337; i++)
        samples.push_back(prng.uniform());
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, underflow_bin_above_expect_empty) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, underflow_bin_exact_expect_empty) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.0, 0.1, 0.2, 0.3};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, underflow_bin_below_expect_full) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {-1e-9, 0.1, 0.2, 0.3};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(1u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, overflow_bin_above_expect_full) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0+1e-9};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(1u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, overflow_bin_exact_expect_full) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(1u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, overflow_bin_below_expect_empty) {
    vector<double> bins = Numeric::linspace(0.0, 1.0, 50);
    vector<double> samples = {0.1, 0.2, 0.3, 1.0-1e-9};
    Histogram1D histo(samples, bins);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
}

TEST_F(Histogram1DTest, all_in_one_bin_middle) {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.5);
    Histogram1D histo(samples, bins_edges);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
    ASSERT_EQ(3u, histo.bins.size());
    EXPECT_EQ(0u, histo.bins[0]);
    EXPECT_EQ(42u*1337u, histo.bins[1]);
    EXPECT_EQ(0u, histo.bins[2]);
}

TEST_F(Histogram1DTest, all_in_one_bin_front) {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.15);
    Histogram1D histo(samples, bins_edges);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
    ASSERT_EQ(3u, histo.bins.size());
    EXPECT_EQ(42u*1337u, histo.bins[0]);
    EXPECT_EQ(0u, histo.bins[1]);
    EXPECT_EQ(0u, histo.bins[2]);
}

TEST_F(Histogram1DTest, all_in_one_bin_back) {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    for (unsigned int i = 0u; i < 42u*1337u; i++)
        samples.push_back(0.75);
    Histogram1D histo(samples, bins_edges);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
    ASSERT_EQ(3u, histo.bins.size());
    EXPECT_EQ(0u, histo.bins[0]);
    EXPECT_EQ(0u, histo.bins[1]);
    EXPECT_EQ(42u*1337u, histo.bins[2]);
}

TEST_F(Histogram1DTest, arg_max) {
    // bins    |   0   |   1     |     2    |
    // edges  0.0   0.3333   0.666666      1.0
    //             2       6           1
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.9};
    Histogram1D histo(samples, bins_edges);
    EXPECT_EQ(0u, histo.underflow_bin);
    EXPECT_EQ(0u, histo.overflow_bin);
    ASSERT_EQ(3u, histo.bins.size());
    EXPECT_EQ(2u, histo.bins[0]);
    EXPECT_EQ(5u, histo.bins[1]);
    EXPECT_EQ(1u, histo.bins[2]);
    EXPECT_EQ(1u, histo.arg_max());
}

TEST_F(Histogram1DTest, arg_max_empty) {
    vector<double> bins_edges;
    vector<double> samples;
    EXPECT_THROW(
        Histogram1D histo(samples, bins_edges),
        std::invalid_argument);
}

TEST_F(Histogram1DTest, arg_max_empty_sample) {
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    Histogram1D histo(samples, bins_edges);
    EXPECT_EQ(0u, histo.arg_max());
}

TEST_F(Histogram1DTest, arg_max_empty_edges) {
    vector<double> bins_edges;
    vector<double> samples = {0.1, 0.2, 0.5};
    EXPECT_THROW(
        Histogram1D histo(samples, bins_edges),
        std::invalid_argument);
}

TEST_F(Histogram1DTest, mode_empty_sample) {
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples;
    Histogram1D histo(samples, bins_edges);
    EXPECT_NEAR(1./6., histo.mode(), 1e-6);
}

TEST_F(Histogram1DTest, mode_front) {
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.1, 0.1, 0.2, 0.5};
    Histogram1D histo(samples, bins_edges);
    EXPECT_NEAR(1./6., histo.mode(), 1e-6);
}

TEST_F(Histogram1DTest, mode_middle) {
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.5, 0.5, 0.5, 0.7, 0.7};
    Histogram1D histo(samples, bins_edges);
    EXPECT_NEAR(0.5, histo.mode(), 1e-6);
}

TEST_F(Histogram1DTest, mode_back) {
    vector<double> bins_edges = Numeric::linspace(0.0, 1.0, 4);
    vector<double> samples = {0.1, 0.5, 0.7, 0.7};
    Histogram1D histo(samples, bins_edges);
    EXPECT_NEAR(5./6., histo.mode(), 1e-6);
}
