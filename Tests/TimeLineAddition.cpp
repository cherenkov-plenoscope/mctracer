#include "gtest/gtest.h"
#include "Tools/Numeric.h"
#include "Tools/Tools.h"
#include "SignalProcessing/TimeLineAddition.h"
using std::vector;

class TimeLineAddition : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, all_empty) {

	vector<double> first;
	vector<double> second;

	EXPECT_EQ(0u, first.size());
	EXPECT_EQ(0u, second.size());

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, empty_summand) {

	vector<double> first = Numeric::linspace(0.0, 1.0, 100);
	vector<double> second;

	EXPECT_EQ(100u, first.size());
	EXPECT_EQ(0u, second.size());

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
	for(unsigned int i=0; i<first.size(); i++)
		EXPECT_EQ(first_before_adding[i], first[i]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, empty_base_summand) {

	vector<double> first;
	vector<double> second = Numeric::linspace(0.0, 1.0, 100);

	EXPECT_EQ(0u, first.size());
	EXPECT_EQ(100u, second.size());

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_front) {

	vector<double> first(3, 0.0);
	vector<double> second(1, 1.0);

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_middle) {

	vector<double> first(3, 0.0);
	vector<double> second(1, 1.0);

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 1);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_end) {

	vector<double> first(3, 0.0);
	vector<double> second(1, 1.0);

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 2);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_out_of_range) {

	vector<double> first(3, 0.0);
	vector<double> second(1, 1.0);

	vector<double> first_before_adding = first; 

	SignalProcessing::add_second_to_first_at(&first, &second, 3);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, 0);

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front_plus_1) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, 1);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front_minus_1) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, -1);

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_middle) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, 2);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
	EXPECT_EQ(1.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_end) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, 3);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(1.0, first[3]);
	EXPECT_EQ(1.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_partly_beyond_end) {

	vector<double> first(5, 0.0);
	vector<double> second(2, 1.0);

	SignalProcessing::add_second_to_first_at(&first, &second, 4);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(1.0, first[4]);
}