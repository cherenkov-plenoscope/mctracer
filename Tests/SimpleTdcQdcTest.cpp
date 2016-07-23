#include "gtest/gtest.h"
#include "SignalProcessing/PreTrigger.h"
#include "SignalProcessing/SimpleTDCQDC.h"
#include "Core/SimulationTruth.h"
using std::vector;
using std::cout;


class SimpleTdcQdcTest : public ::testing::Test {
public:
	vector<ElectricPulse> pulses_with_arrival_times(
		const vector<double> &arrival_moments
	) {

		vector<ElectricPulse> pulses;
		for(double arrival_moment: arrival_moments)
			pulses.push_back(
				ElectricPulse(
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
	vector<ElectricPulse> pulses = pulses_with_arrival_times(arrival_moments);

	const double integration_time_window = 0.5;

	vector<uint> enclosure = 
		SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
			pulses,
			integration_time_window
		);

	vector<uint> expected_enclosure = {
		1,   1,   5,   5,   5,   5,   5,   5,   5,   4,   3,   2,   1,   1,   1
	};

	ASSERT_EQ(expected_enclosure.size(), enclosure.size());
	for(uint i=0; i<enclosure.size(); i++)
		EXPECT_EQ(enclosure.at(i), expected_enclosure.at(i));
}
//------------------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, tdcqdc_empty) {

	vector<double> arrival_moments;
	vector<ElectricPulse> pulses = pulses_with_arrival_times(arrival_moments);

	const double integration_time_window = 0.5;

	vector<uint> enclosure = 
		SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
			pulses,
			integration_time_window
		);

	EXPECT_EQ(0u, enclosure.size());
}
//------------------------------------------------------------------------------