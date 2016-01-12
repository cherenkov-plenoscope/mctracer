#include "gtest/gtest.h"
#include "Core/Histogram1D.h"
#include "Core/Random/Random.h"
#include "Tools/Tools.h"

class Histogram1DTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, empty_bin_edges) {

    std::vector<double> bins_edges;
    EXPECT_EQ(0, bins_edges.size());

    Random::Mt19937 prng(0);
    std::vector<double> samples;

    for(uint i=0; i<42*1337; i++)
        samples.push_back(prng.uniform());

    Histogram1D histo(samples, bins_edges);

    EXPECT_EQ(0, histo.bins.size());
    EXPECT_EQ(42*1337, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, init) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);

    Random::Mt19937 prng(0);
    std::vector<double> samples;

    for(uint i=0; i<42*1337; i++)
        samples.push_back(prng.uniform());

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, underflow_bin_above_expect_empty) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {0.1, 0.2, 0.3};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, underflow_bin_exact_expect_empty) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {0.0, 0.1, 0.2, 0.3};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, underflow_bin_below_expect_full) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {-1e-9, 0.1, 0.2, 0.3};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(1, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, overflow_bin_above_expect_full) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {0.1, 0.2, 0.3, 1.0+1e-9};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(1, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, overflow_bin_exact_expect_full) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {0.1, 0.2, 0.3, 1.0};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(1, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, overflow_bin_below_expect_empty) {

    std::vector<double> bins = numeric::linspace(0.0, 1.0, 50);
    std::vector<double> samples = {0.1, 0.2, 0.3, 1.0-1e-9};

    Histogram1D histo(samples, bins);
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, all_in_one_bin_middle) {

    // bins    |   0   |   1     |     2    | 
    // edges  0.0   0.3333   0.666666      1.0
    std::vector<double> bins_edges =  numeric::linspace(0.0, 1.0, 4);

    std::vector<double> samples;
    for(uint i=0; i<42*1337; i++)
        samples.push_back(0.5);

    Histogram1D histo(samples, bins_edges);
    
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
    ASSERT_EQ(3, histo.bins.size());
    EXPECT_EQ(0, histo.bins[0]);
    EXPECT_EQ(42*1337, histo.bins[1]);
    EXPECT_EQ(0, histo.bins[2]);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, all_in_one_bin_front) {

    // bins    |   0   |   1     |     2    | 
    // edges  0.0   0.3333   0.666666      1.0
    std::vector<double> bins_edges =  numeric::linspace(0.0, 1.0, 4);

    std::vector<double> samples;
    for(uint i=0; i<42*1337; i++)
        samples.push_back(0.15);

    Histogram1D histo(samples, bins_edges);
    
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
    ASSERT_EQ(3, histo.bins.size());
    EXPECT_EQ(42*1337, histo.bins[0]);
    EXPECT_EQ(0, histo.bins[1]);
    EXPECT_EQ(0, histo.bins[2]);
}
//------------------------------------------------------------------------------
TEST_F(Histogram1DTest, all_in_one_bin_back) {

    // bins    |   0   |   1     |     2    | 
    // edges  0.0   0.3333   0.666666      1.0
    std::vector<double> bins_edges =  numeric::linspace(0.0, 1.0, 4);

    std::vector<double> samples;
    for(uint i=0; i<42*1337; i++)
        samples.push_back(0.75);

    Histogram1D histo(samples, bins_edges);
    
    EXPECT_EQ(0, histo.underflow_bin);
    EXPECT_EQ(0, histo.overflow_bin);
    ASSERT_EQ(3, histo.bins.size());
    EXPECT_EQ(0, histo.bins[0]);
    EXPECT_EQ(0, histo.bins[1]);
    EXPECT_EQ(42*1337, histo.bins[2]);
}