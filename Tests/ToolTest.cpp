// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "gtest/gtest.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
using namespace relleums;

class Tools : public ::testing::Test {};

TEST_F(Tools, deg2rad_rad2deg) {
    for (int i = -721; i < 721; i++) {
        double angle_in_deg = static_cast<double>(i);
        EXPECT_NEAR(angle_in_deg, rad2deg(deg2rad(angle_in_deg)), 1e-9);
    }
}

TEST_F(Tools, deg2rad) {
    EXPECT_NEAR(0.0, deg2rad(0.0), 1e-9);
    EXPECT_NEAR(M_PI/2.0, deg2rad(90.0), 1e-9);
    EXPECT_NEAR(M_PI, deg2rad(180.0), 1e-9);
    EXPECT_NEAR(-M_PI/2.0, deg2rad(-90.0), 1e-9);
    EXPECT_NEAR(-M_PI, deg2rad(-180.0), 1e-9);
    EXPECT_NEAR(2.0*M_PI, deg2rad(360.0), 1e-9);
}

TEST_F(Tools, rad2deg) {
    EXPECT_NEAR(0.0, rad2deg(0.0*M_PI), 1e-9);
    EXPECT_NEAR(45.0, rad2deg(0.25*M_PI), 1e-9);
    EXPECT_NEAR(90.0, rad2deg(0.5*M_PI), 1e-9);
    EXPECT_NEAR(180.0, rad2deg(1.0*M_PI), 1e-9);
    EXPECT_NEAR(180.0e3, rad2deg(1.0e3*M_PI), 1e-9);
}

TEST_F(Tools, odd_and_even) {
    EXPECT_TRUE(is_even(0));
    EXPECT_TRUE(is_even(2));
    EXPECT_TRUE(is_even(-2));
    EXPECT_TRUE(is_even(1214214));

    EXPECT_TRUE(is_odd(-1));
    EXPECT_TRUE(is_odd(-3));
    EXPECT_TRUE(is_odd(-7));
    EXPECT_TRUE(is_odd(-9));

    EXPECT_FALSE(is_even(3));
    EXPECT_FALSE(is_even(-1));
    EXPECT_FALSE(is_even(1337));
    EXPECT_FALSE(is_even(23));

    EXPECT_FALSE(is_odd(2));
    EXPECT_FALSE(is_odd(0));
    EXPECT_FALSE(is_odd(42));
    EXPECT_FALSE(is_odd(-46));
}

TEST_F(Tools, get_solid_angle_for_opening_angle) {
    EXPECT_NEAR(0.0, get_solid_angle_for_opening_angle(0.0), 1e-9);
    EXPECT_NEAR(2*M_PI, get_solid_angle_for_opening_angle(M_PI/2.0), 1e-9);
    EXPECT_NEAR(4*M_PI, get_solid_angle_for_opening_angle(M_PI), 1e-9);
}
