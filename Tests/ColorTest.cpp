// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/Color.h"

class ColorTest : public ::testing::Test {};

TEST_F(ColorTest, default_ctor) {
    Color c;
    EXPECT_EQ(c.r, 0);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST_F(ColorTest, reflection_mix) {
    Color base(0, 0, 0);
    Color green(0, 100, 0);
    const double reflectivity = 0.5;
    base.reflection_mix(green, reflectivity);
    EXPECT_EQ(base.r, 0);
    EXPECT_EQ(base.g, 50);
    EXPECT_EQ(base.b, 0);
}

TEST_F(ColorTest, ctor_mix) {
    std::vector<Color> rainbow;
    rainbow.push_back(Color(100, 0, 0));
    rainbow.push_back(Color(0, 100, 0));
    rainbow.push_back(Color(0, 0, 100));
    Color mix = Color(rainbow);
    EXPECT_NEAR(mix.r, 33, 1);
    EXPECT_NEAR(mix.g, 33, 1);
    EXPECT_NEAR(mix.b, 33, 1);
}
