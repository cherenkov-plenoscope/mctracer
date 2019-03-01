// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "plenoscope/calibration/LixelStatistics.h"


TEST_CASE("PlenoscopeLixelStatisticsTest: default_ctor", "[merlict]") {
    plenoscope::calibration::LixelStatistic stat;

    CHECK(0.0 == stat.efficiency);
    CHECK(0.0 == stat.efficiency_std);
    CHECK(0.0 == stat.cx_mean);
    CHECK(0.0 == stat.cx_std);
    CHECK(0.0 == stat.cy_mean);
    CHECK(0.0 == stat.cy_std);
    CHECK(0.0 == stat.x_mean);
    CHECK(0.0 == stat.x_std);
    CHECK(0.0 == stat.y_mean);
    CHECK(0.0 == stat.y_std);
    CHECK(0.0 == stat.time_delay_mean);
    CHECK(0.0 == stat.time_delay_std);
}

TEST_CASE("PlenoscopeLixelStatisticsTest: write_and_read_binary", "[merlict]") {
    const unsigned int number_of_lixels = 1337;
    std::vector<plenoscope::calibration::LixelStatistic> lixel_stats;

    for (unsigned int i = 0; i < number_of_lixels; i++) {
        plenoscope::calibration::LixelStatistic stat;
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

    const std::string path = "InOut/my_stats.LixelStatistics.bin";
    plenoscope::calibration::write(lixel_stats, path);

    std::vector<plenoscope::calibration::LixelStatistic> lixel_stats_in =
        plenoscope::calibration::read(path);

    CHECK(number_of_lixels == lixel_stats_in.size());

    for (unsigned int i = 0; i < number_of_lixels; i++) {
        CHECK(lixel_stats_in.at(i).efficiency == Approx(i*1.000).margin(1e-4));
        CHECK(lixel_stats_in.at(i).efficiency_std == Approx(i*1.001).margin(1e-4));
        CHECK(lixel_stats_in.at(i).cx_mean == Approx(i*1.002).margin(1e-4));
        CHECK(lixel_stats_in.at(i).cx_std == Approx(i*1.003).margin(1e-4));
        CHECK(lixel_stats_in.at(i).cy_mean == Approx(i*1.004).margin(1e-4));
        CHECK(lixel_stats_in.at(i).cy_std == Approx(i*1.005).margin(1e-4));
        CHECK(lixel_stats_in.at(i).x_mean == Approx(i*1.006).margin(1e-4));
        CHECK(lixel_stats_in.at(i).x_std == Approx(i*1.007).margin(1e-4));
        CHECK(lixel_stats_in.at(i).y_mean == Approx(i*1.008).margin(1e-4));
        CHECK(lixel_stats_in.at(i).y_std == Approx(i*1.009).margin(1e-4));
        CHECK(lixel_stats_in.at(i).time_delay_mean == Approx(i*1.010).margin(1e-4));
        CHECK(lixel_stats_in.at(i).time_delay_std == Approx(i*1.011).margin(1e-4));
    }
}

TEST_CASE("PlenoscopeLixelStatisticsTest: read_non_existing_binary_file", "[merlict]") {
    CHECK_THROWS_AS(plenoscope::calibration::read("InOut/non_existing_file.bin"), std::runtime_error);
}

TEST_CASE("PlenoscopeLixelStatisticsTest: size_is_just_a_plain_struct", "[merlict]") {
    CHECK(12u*4u == sizeof(plenoscope::calibration::LixelStatistic));
}
