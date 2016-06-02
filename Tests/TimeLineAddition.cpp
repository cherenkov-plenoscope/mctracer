#include "gtest/gtest.h"
#include "Tools/Tools.h"

class TimeLineAddition : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, all_empty) {

	std::vector<double> first;
	std::vector<double> second;

	EXPECT_EQ(0u, first.size());
	EXPECT_EQ(0u, second.size());

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, empty_summand) {

	std::vector<double> first = numeric::linspace(0.0, 1.0, 100);
	std::vector<double> second;

	EXPECT_EQ(100u, first.size());
	EXPECT_EQ(0u, second.size());

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
	for(uint i=0; i<first.size(); i++)
		EXPECT_EQ(first_before_adding[i], first[i]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, empty_base_summand) {

	std::vector<double> first;
	std::vector<double> second = numeric::linspace(0.0, 1.0, 100);

	EXPECT_EQ(0u, first.size());
	EXPECT_EQ(100u, second.size());

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_front) {

	std::vector<double> first(3, 0.0);
	std::vector<double> second(1, 1.0);

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 0);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_middle) {

	std::vector<double> first(3, 0.0);
	std::vector<double> second(1, 1.0);

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 1);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_end) {

	std::vector<double> first(3, 0.0);
	std::vector<double> second(1, 1.0);

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 2);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, simple_addition_out_of_range) {

	std::vector<double> first(3, 0.0);
	std::vector<double> second(1, 1.0);

	std::vector<double> first_before_adding = first; 

	add_second_to_first_at(&first, &second, 3);

	ASSERT_EQ(first_before_adding.size(), first.size());

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, 0);

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front_plus_1) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, 1);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(1.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_front_minus_1) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, -1);

	EXPECT_EQ(1.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_middle) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, 2);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(1.0, first[2]);
	EXPECT_EQ(1.0, first[3]);
	EXPECT_EQ(0.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_end) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, 3);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(1.0, first[3]);
	EXPECT_EQ(1.0, first[4]);
}
//------------------------------------------------------------------------------
TEST_F(TimeLineAddition, complex_addition_partly_beyond_end) {

	std::vector<double> first(5, 0.0);
	std::vector<double> second(2, 1.0);

	add_second_to_first_at(&first, &second, 4);

	EXPECT_EQ(0.0, first[0]);
	EXPECT_EQ(0.0, first[1]);
	EXPECT_EQ(0.0, first[2]);
	EXPECT_EQ(0.0, first[3]);
	EXPECT_EQ(1.0, first[4]);
}