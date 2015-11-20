#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "HexGridAnnulus.h"

class HexGridAnnulusTest : public ::testing::Test {
};
//----------------------------------------------------------------------
TEST_F(HexGridAnnulusTest, Cells_are_inside_boundary) {

    for(double outer_radius = 1.65; outer_radius<16.5; outer_radius++) {
        HexGridAnnulus my_grid(outer_radius, 0.62);

    std::vector<Vector3D> grid = my_grid.get_grid();

    for(Vector3D cell : grid)
        EXPECT_TRUE(outer_radius > cell.norm());
    }
}
//----------------------------------------------------------------------