// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_HEXGRIDANNULUS_H_
#define SCENERY_GEOMETRY_HEXGRIDANNULUS_H_

#include <string>
#include <vector>
#include "merlict/Vec3.h"

namespace merlict {

class HexGridAnnulus{
    Vec3 unit_hex_a;
    Vec3 unit_hex_b;
    std::vector<Vec3> grid;

 public:
    HexGridAnnulus(
        const double outer_radius,
        const double spacing);
    HexGridAnnulus(
        const double outer_radius,
        const double inner_radius,
        const double spacing);
    std::vector<Vec3> get_grid()const;
    std::string str()const;

 private:
    void init_unit_vectors_hex_grid_with_length(const double spacing);
    void init_grid(
        const double outer_radius,
        const double inner_radius,
        const double spacing);
};

}  // namespace merlict

#endif  // SCENERY_GEOMETRY_HEXGRIDANNULUS_H_
