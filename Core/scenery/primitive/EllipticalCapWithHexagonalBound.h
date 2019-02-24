// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_ELLIPTICALCAPWITHHEXAGONALBOUND_H_
#define SCENERY_PRIMITIVE_ELLIPTICALCAPWITHHEXAGONALBOUND_H_

#include <vector>
#include <string>
#include "Core/scenery/geometry/EllipticalCapRayIntersectionEquation.h"
#include "Core/scenery/geometry/HexagonalPrismZ.h"
#include "Core/scenery/geometry/SurfaceWithOuterPrismBound.h"

namespace relleums {

class EllipticalCapWithHexagonalBound :public SurfaceWithOuterPrismBound {
 protected:
    HexagonalPrismZ hexBounds;
    double focal_length;
    double ratio_x_to_y_curvature_radius;
    double X_curvature_radius;
    double Y_curvature_radius;
    double Z_curvature_radius;
    double iXX;
    double iYY;
    double iZZ;
    double hex_bound_rotation;
    double outer_hex_radius;

 public:
    void set_focalLength_radiiRatio_hexAngel_hexRadius(
        const double focal_length,
        const double ratio_x_to_y_curvature_radius,
        const double hex_bound_rotation,
        const double outer_hex_radius);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    void restrict_outer_hex_radius_to_curvature_radius();
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_ELLIPTICALCAPWITHHEXAGONALBOUND_H_
