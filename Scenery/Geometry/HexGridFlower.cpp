// Copyright 2014 Sebastian A. Mueller
#include "Scenery/Geometry/HexGridFlower.h"
#include <math.h>
#include "Core/tools.h"

namespace relleums {

HexGridFlower::HexGridFlower(
    const double outer_hex_grid_radius,
    unsigned int facet_count_on_outer_diameter
) {
    // ensure diagonal count is odd
    if (is_even(facet_count_on_outer_diameter))
        facet_count_on_outer_diameter++;
    const unsigned int facet_row_count_on_flat2flat_diameter =
        facet_count_on_outer_diameter;
    const double inner_hex_grid_radius = outer_hex_grid_radius*(sqrt(3.0)/2.0);
    const double inner_hex_grid_diameter = 2.0*inner_hex_grid_radius;
    const double outer_hex_facet_radius =
        inner_hex_grid_diameter/(
            0.5*(
                3.0*static_cast<double>(facet_row_count_on_flat2flat_diameter)
                + 1.0));
    const double inner_hex_facet_radius =
        outer_hex_facet_radius*(sqrt(3.0)/2.0);
    facet_spacing = 2.0*inner_hex_facet_radius;
    hex_prism.set_outer_radius(outer_hex_grid_radius);
    init_grid();
}

void HexGridFlower::init_grid() {
    init_unit_vectors_hex_grid_with_length();
    int sample_radius = 2.0*floor(hex_prism.get_outer_radius()/facet_spacing);
    for (int a = -sample_radius; a <= sample_radius; a++) {
        for (int b = -sample_radius; b <= sample_radius; b++) {
            Vec3 cell_ab = unit_hex_a*a + unit_hex_b*b;
            if (hex_prism.is_inside(&cell_ab))
                grid.push_back(cell_ab);
        }
    }
}

std::vector<Vec3> HexGridFlower::get_grid()const {
    return grid;
}

void HexGridFlower::init_unit_vectors_hex_grid_with_length() {
    unit_hex_b = VEC3_UNIT_X*facet_spacing;
    unit_hex_a = (
        VEC3_UNIT_X*0.5+VEC3_UNIT_Y*sqrt(3.0)/2.0)*facet_spacing;
}

double HexGridFlower::get_facet_spacing()const {
    return facet_spacing;
}

}  // namespace relleums
