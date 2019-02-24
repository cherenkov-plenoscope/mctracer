// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/scenery/geometry/HexGridAnnulus.h"

using namespace relleums;

class HexGridAnnulusTest : public ::testing::Test {};

TEST_F(HexGridAnnulusTest, Cells_are_inside_boundary) {
    for (double outer_radius = 1.65; outer_radius < 16.5; outer_radius++) {
        HexGridAnnulus my_grid(outer_radius, 0.62);
    std::vector<Vec3> grid = my_grid.get_grid();
    for (Vec3 cell : grid)
        EXPECT_TRUE(outer_radius > cell.norm());
    }
}