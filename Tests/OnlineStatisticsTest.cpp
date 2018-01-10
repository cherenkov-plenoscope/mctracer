// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Tools/OnlineStatistics.h"
#include "Core/Random/Random.h"
#include "Tools/Numeric.h"
#include "Tools/Tools.h"
using std::vector;

class OnlineStatisticsTest : public ::testing::Test {};

TEST_F(OnlineStatisticsTest, few_samples) {
    vector<double> vals = {1.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    EXPECT_EQ(ov.mean(), 1.0);
    EXPECT_EQ(ov.number_of_samples(), 1);
    EXPECT_TRUE(ov.variance() != ov.variance());  // is nan
    EXPECT_TRUE(ov.stddev() != ov.stddev());  // is nan
}

TEST_F(OnlineStatisticsTest, online_variance) {
    vector<double> vals;
    Random::Mt19937 prng(0);
    OnlineStatistics ov;
    for (unsigned int i = 0; i < 1000000; i++) {
        const double r = prng.uniform();
        vals.push_back(r);
        ov.add(r);
    }
    double stddev_using_online = ov.stddev();
    double stddev_using_classic = Numeric::stddev(vals);
    EXPECT_NEAR(stddev_using_online, stddev_using_classic, 1e-6);
    EXPECT_EQ(ov.number_of_samples(), 1000000);
}

TEST_F(OnlineStatisticsTest, online_variance_simple_numbers) {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    double stddev_using_online = ov.stddev();
    double stddev_using_classic = Numeric::stddev(vals);
    EXPECT_NEAR(stddev_using_online, stddev_using_classic, 1e-6);
    EXPECT_EQ(ov.number_of_samples(), 7);
}

TEST_F(OnlineStatisticsTest, sum) {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    EXPECT_EQ(ov.sum(), 21.0);
}

TEST_F(OnlineStatisticsTest, mean) {
    vector<double> vals = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    OnlineStatistics ov;
    for (unsigned int i = 0; i < vals.size(); i++)
        ov.add(vals.at(i));
    EXPECT_EQ(ov.mean(), 3.0);
}
