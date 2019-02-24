// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/scenery/SensorMap.h"
using std::string;
using std::vector;
using namespace relleums;

class SensorMapTest : public ::testing::Test {};

TEST_F(SensorMapTest, init_one) {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    EXPECT_EQ(smap.sensors.size(), 1u);
}

TEST_F(SensorMapTest, init_two) {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    smap.add(1u, &hans);
    EXPECT_EQ(smap.sensors.size(), 2u);
}

TEST_F(SensorMapTest, add_zeroth_with_id_uneq_zero) {
    SensorMap smap;
    Frame hans;
    EXPECT_THROW(smap.add(1u, &hans);, std::invalid_argument);
}

TEST_F(SensorMapTest, add_first_with_id_uneq_one) {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    EXPECT_THROW(smap.add(2u, &hans);, std::invalid_argument);
}
