// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_HEXGRIDFLOWER_H_
#define SCENERY_GEOMETRY_HEXGRIDFLOWER_H_

#include <string>
#include <vector>
#include "Core/Vec3.h"
#include "HexagonalPrismZ.h"

class HexGridFlower{
    Vec3 unit_hex_a;
    Vec3 unit_hex_b;
    std::vector<Vec3> grid;
    HexagonalPrismZ hex_prism;
    double facet_spacing;

 public:
    HexGridFlower(
        const double outer_hex_grid_radius,
        unsigned int facet_count_on_outer_diameter);
    double get_facet_spacing()const;
    std::vector<Vec3> get_grid()const;

 private:
    void init_unit_vectors_hex_grid_with_length();
    void init_grid();
};

#endif  // SCENERY_GEOMETRY_HEXGRIDFLOWER_H_
