// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/Primitive/Disc.h"

class DiscTest : public ::testing::Test {};

TEST_F(DiscTest, set_Disc) {
    Disc D;
    D.set_radius(42.0);
    EXPECT_EQ(42.0, D.get_bounding_sphere_radius());
}

TEST_F(DiscTest, set_Disc_negative_radius) {
    EXPECT_THROW(
        Disc D;
        D.set_radius(-42.0),
        std::invalid_argument);
}
