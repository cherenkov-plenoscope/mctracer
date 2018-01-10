// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "SignalProcessing/PreTrigger.h"
#include "SignalProcessing/SimpleTDCQDC.h"
using std::vector;
using std::cout;
namespace SP = SignalProcessing;

class PreTriggerTest : public ::testing::Test {};

TEST_F(PreTriggerTest, empty_time_line) {
    std::vector<double> arrival_moments;

    EXPECT_EQ(0u, arrival_moments.size());

    SP::PreTrigger::Config conf;
    conf.time_window = 1e-9;
    conf.threshold = 1;
    EXPECT_FALSE(SP::PreTrigger::might_trigger(&arrival_moments, conf));

    conf.threshold = 0;
    EXPECT_TRUE(SP::PreTrigger::might_trigger(&arrival_moments, conf));
}

TEST_F(PreTriggerTest, single_in_time_line) {
    std::vector<double> arrival_moments = {0.0};

    SP::PreTrigger::Config conf;
    conf.time_window = 1e-9;
    conf.threshold = 1;
    EXPECT_TRUE(SP::PreTrigger::might_trigger(&arrival_moments, conf));

    conf.threshold = 2;
    EXPECT_FALSE(SP::PreTrigger::might_trigger(&arrival_moments, conf));
}

TEST_F(PreTriggerTest, even_distributed) {
    std::vector<double> arrival_moments = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};

    SP::PreTrigger::Config conf;
    conf.time_window = 0.9;
    conf.threshold = 2;
    EXPECT_FALSE(SP::PreTrigger::might_trigger(&arrival_moments, conf));
    conf.time_window = 1.01;
    EXPECT_TRUE(SP::PreTrigger::might_trigger(&arrival_moments, conf));
}

TEST_F(PreTriggerTest, dense_center_distributed) {
    std::vector<double> arrival_moments = {
        0.0, 1.0, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 4.0,
        5.0
    };

    SP::PreTrigger::Config conf;
    conf.time_window = 1.0;
    conf.threshold = 12;
    EXPECT_FALSE(SP::PreTrigger::might_trigger(&arrival_moments, conf));

    conf.time_window = 0.5;
    conf.threshold = 4;
    EXPECT_TRUE(SP::PreTrigger::might_trigger(&arrival_moments, conf));
}
