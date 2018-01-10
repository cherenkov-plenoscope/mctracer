// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Plenoscope/Calibration/LixelStatistics.h"
using std::vector;

class PlenoscopeLixelStatisticsTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(PlenoscopeLixelStatisticsTest, default_ctor) {
    Plenoscope::Calibration::LixelStatistic stat;

    EXPECT_EQ(stat.efficiency, 0.0);
    EXPECT_EQ(stat.efficiency_std, 0.0);
    EXPECT_EQ(stat.cx_mean, 0.0);
    EXPECT_EQ(stat.cx_std, 0.0);
    EXPECT_EQ(stat.cy_mean, 0.0);
    EXPECT_EQ(stat.cy_std, 0.0);
    EXPECT_EQ(stat.x_mean, 0.0);
    EXPECT_EQ(stat.x_std, 0.0);
    EXPECT_EQ(stat.y_mean, 0.0);
    EXPECT_EQ(stat.y_std, 0.0);
    EXPECT_EQ(stat.time_delay_mean, 0.0);
    EXPECT_EQ(stat.time_delay_std, 0.0);
}
//----------------------------------------------------------------------
TEST_F(PlenoscopeLixelStatisticsTest, write_and_read_binary) {

    const unsigned int number_of_lixels = 1337;
    vector<Plenoscope::Calibration::LixelStatistic> lixel_stats;

    for(unsigned int i=0; i<number_of_lixels; i++) {
        Plenoscope::Calibration::LixelStatistic stat;
        stat.efficiency =       i*1.000;
        stat.efficiency_std =   i*1.001;
        stat.cx_mean =          i*1.002;
        stat.cx_std =           i*1.003;
        stat.cy_mean =          i*1.004;
        stat.cy_std =           i*1.005;
        stat.x_mean =           i*1.006;
        stat.x_std =            i*1.007;
        stat.y_mean =           i*1.008; 
        stat.y_std =            i*1.009;
        stat.time_delay_mean =  i*1.010;
        stat.time_delay_std =   i*1.011;    
        lixel_stats.push_back(stat);
    }
    
    const string path = "InOut/my_stats.LixelStatistics.bin";
    Plenoscope::Calibration::write(lixel_stats, path);

    vector<Plenoscope::Calibration::LixelStatistic> lixel_stats_in = 
        Plenoscope::Calibration::read(path);

    EXPECT_EQ(lixel_stats_in.size(), number_of_lixels);

    for(unsigned int i=0; i<number_of_lixels; i++) {
        EXPECT_NEAR(lixel_stats_in.at(i).efficiency,        i*1.000, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).efficiency_std,    i*1.001, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).cx_mean,           i*1.002, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).cx_std,            i*1.003, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).cy_mean,           i*1.004, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).cy_std,            i*1.005, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).x_mean,            i*1.006, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).x_std,             i*1.007, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).y_mean,            i*1.008, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).y_std,             i*1.009, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).time_delay_mean,   i*1.010, 1e-4);
        EXPECT_NEAR(lixel_stats_in.at(i).time_delay_std,    i*1.011, 1e-4);
    }
}
//----------------------------------------------------------------------
TEST_F(PlenoscopeLixelStatisticsTest, read_non_existing_binary_file) {

    EXPECT_THROW(
        Plenoscope::Calibration::read("InOut/non_existing_file.bin");,
        std::runtime_error
    );
}
//----------------------------------------------------------------------
TEST_F(PlenoscopeLixelStatisticsTest, size_is_just_a_plain_struct) {

    EXPECT_EQ(sizeof(Plenoscope::Calibration::LixelStatistic), 12u*4u);
}