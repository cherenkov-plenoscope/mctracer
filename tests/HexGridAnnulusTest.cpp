// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/HexGridAnnulus.h"

using namespace relleums;



TEST_CASE("HexGridAnnulusTest: Cells_are_inside_boundary", "[mctracer]") {
    for (double outer_radius = 1.65; outer_radius < 16.5; outer_radius++) {
        HexGridAnnulus my_grid(outer_radius, 0.62);
    std::vector<Vec3> grid = my_grid.get_grid();
    for (Vec3 cell : grid)
        CHECK(outer_radius > cell.norm());
    }
}
