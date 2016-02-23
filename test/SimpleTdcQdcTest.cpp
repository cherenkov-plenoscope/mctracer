#include "PreTrigger.h"
#include "SimpleTDCQDC.h"
using std::vector;
using std::cout;


class SimpleTdcQdcTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, tdcqdc1) {

	std::vector<double> arrival_moments = {
	//  1    1    5    5    5    5    5    5    5    4    3    2    1    1    1
		0.0, 1.0, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 4.0, 5.0
	};

	const double integration_time_window = 0.5;

	vector<uint> enclosure = 
		SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
			arrival_moments,
			integration_time_window
		);

	vector<uint> expected_enclosure = {
		1,   1,   5,   5,   5,   5,   5,   5,   5,   4,   3,   2,   1,   1,   1
	};

	ASSERT_EQ(expected_enclosure.size(), enclosure.size());
	for(uint i=0; i<enclosure.size(); i++)
		EXPECT_EQ(enclosure.at(i), expected_enclosure.at(i));
}
//----------------------------------------------------------------------
TEST_F(SimpleTdcQdcTest, tdcqdc_empty) {

	std::vector<double> arrival_moments;

	const double integration_time_window = 0.5;

	vector<uint> enclosure = 
		SimpleTdcQdc::convolution_of_arrival_times_and_integration_window(
			arrival_moments,
			integration_time_window
		);

	EXPECT_EQ(0, enclosure.size());
}