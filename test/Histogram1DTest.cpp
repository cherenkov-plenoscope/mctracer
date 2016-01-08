#include "gtest/gtest.h"
#include "Core/Histogram1D.h"
#include "Core/Random/Random.h"
#include "Tools/Tools.h"

class Histogram1DTest : public ::testing::Test {};
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