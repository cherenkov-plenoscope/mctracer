// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_SPHERECAPWITHHEXAGONALBOUND_H_
#define SCENERY_PRIMITIVE_SPHERECAPWITHHEXAGONALBOUND_H_

#include <vector>
#include <string>
#include "scenery/geometry/SphericalCapRayIntersectionEquation.h"
#include "scenery/geometry/HexagonalPrismZ.h"
#include "scenery/geometry/SurfaceWithOuterPrismBound.h"

namespace relleums {

class SphereCapWithHexagonalBound :public SurfaceWithOuterPrismBound {
 protected:
    HexagonalPrismZ hexBounds;
    double curvature_radius;
    double outer_hex_radius;

 public:
    void set_curvature_radius_and_outer_hex_radius(
        const double curvature_radius,
        const double outer_hex_radius);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;
    double get_focal_length()const;

 private:
    void restrict_outer_hex_radius_to_curvature_radius();
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_SPHERECAPWITHHEXAGONALBOUND_H_
