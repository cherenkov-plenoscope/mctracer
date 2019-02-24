// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/scenery/geometry/ThinLensEquation.h"

using namespace relleums;

class ThinLensEquationTest : public ::testing::Test {};

TEST_F(ThinLensEquationTest, obj_dist_large_vs_focal_dist) {
    double o = 1.0e9;
    double f = 0.1337;
    EXPECT_NEAR(
        f,
        ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o),
        1e-9);
}

TEST_F(ThinLensEquationTest, fact_set_up_checked_by_hand) {
    double o = 10.0e3;
    double f = 4.889;
    EXPECT_NEAR(
        f + 2.391401256e-3,
        ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o),
        1e-6);
}

TEST_F(ThinLensEquationTest, two_f_cross_check) {
    double f = 4.889;
    double o = 2.0*f;
    double i = 2.0*f;
    EXPECT_NEAR(
        i,
        ThinLensEquation::get_image_dist_given_focal_and_object_dist(f, o),
        1e-9);
    EXPECT_NEAR(
        o,
        ThinLensEquation::get_object_dist_given_focal_and_image_dist(f, i),
        1e-9);
    EXPECT_NEAR(
        f,
        ThinLensEquation::get_focal_dist_given_object_and_image_dist(o, i),
        1e-9);
}
