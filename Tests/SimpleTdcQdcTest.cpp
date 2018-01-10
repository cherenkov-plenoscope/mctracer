// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "SignalProcessing/PreTrigger.h"
#include "SignalProcessing/SimpleTDCQDC.h"
#include "Core/SimulationTruth.h"
using std::vector;
using std::cout;
namespace SP = SignalProcessing;

class SimpleTdcQdcTest : public ::testing::Test {
public:
    vector<SP::ElectricPulse> pulses_with_arrival_times(
        const vector<double> &arrival_moments
    ) {

        vector<SP::ElectricPulse> pulses;
        for(double arrival_moment: arrival_moments)
            pulses.push_back(
                SP::ElectricPulse(
                    arrival_moment,
                    SimulationTruth::MCTRACER_DEFAULT
                )
            );

        return pulses;
    }
};
//------------------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, tdcqdc1) {

    vector<double> arrival_moments = {
    //  1    1    5    5    5    5    5    5    5    4    3    2    1    1    1
        0.0, 1.0, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 4.0, 5.0
    };
    vector<SP::ElectricPulse> pulses = pulses_with_arrival_times(arrival_moments);

    const double integration_time_window = 0.5;

    vector<unsigned int> enclosure = 
        SP::SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
            pulses,
            integration_time_window
        );

    vector<unsigned int> expected_enclosure = {
        1,   1,   5,   5,   5,   5,   5,   5,   5,   4,   3,   2,   1,   1,   1
    };

    ASSERT_EQ(expected_enclosure.size(), enclosure.size());
    for(unsigned int i=0; i<enclosure.size(); i++)
        EXPECT_EQ(enclosure.at(i), expected_enclosure.at(i));
}
//------------------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, tdcqdc_empty) {

    vector<double> arrival_moments;
    vector<SP::ElectricPulse> pulses = pulses_with_arrival_times(arrival_moments);

    const double integration_time_window = 0.5;

    vector<unsigned int> enclosure = 
        SP::SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
            pulses,
            integration_time_window
        );

    EXPECT_EQ(0u, enclosure.size());
}
//------------------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, Charge_count_equals_simulation_truth_count) {

    vector<SP::ElectricPulse> pulses;
    pulses.push_back(SP::ElectricPulse(0.0,  1));
    pulses.push_back(SP::ElectricPulse(1.0,  2));
    pulses.push_back(SP::ElectricPulse(2.0,  3)); //
    pulses.push_back(SP::ElectricPulse(2.1,  4)); //
    pulses.push_back(SP::ElectricPulse(2.2,  5)); //
    pulses.push_back(SP::ElectricPulse(2.3,  6)); // 
    pulses.push_back(SP::ElectricPulse(2.4,  7)); //
    pulses.push_back(SP::ElectricPulse(2.5,  8)); //                          
    pulses.push_back(SP::ElectricPulse(2.55, 9)); //
    pulses.push_back(SP::ElectricPulse(2.6, 10));
    pulses.push_back(SP::ElectricPulse(2.7, 11));
    pulses.push_back(SP::ElectricPulse(2.8, 12));
    pulses.push_back(SP::ElectricPulse(2.9, 13));
    pulses.push_back(SP::ElectricPulse(3.0, 14));
    pulses.push_back(SP::ElectricPulse(4.0, 15));
    pulses.push_back(SP::ElectricPulse(5.0, 16));
    pulses.push_back(SP::ElectricPulse(6.0, 17));

    EXPECT_EQ(17u, pulses.size());

    const double integration_time_window = 0.49;

    SP::SimpleTdcQdc::TimeAndCount tac = SP::SimpleTdcQdc::get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
        pulses,
        integration_time_window
    );

    EXPECT_EQ(2.1, tac.time);
    EXPECT_EQ(6u, tac.count);
    EXPECT_EQ(tac.count, tac.simulation_truth_ids.size());
    ASSERT_EQ(6u, tac.simulation_truth_ids.size());

    EXPECT_EQ(4, tac.simulation_truth_ids.at(0));
    EXPECT_EQ(5, tac.simulation_truth_ids.at(1));
    EXPECT_EQ(6, tac.simulation_truth_ids.at(2));
    EXPECT_EQ(7, tac.simulation_truth_ids.at(3));
    EXPECT_EQ(8, tac.simulation_truth_ids.at(4));
    EXPECT_EQ(9, tac.simulation_truth_ids.at(5));
}