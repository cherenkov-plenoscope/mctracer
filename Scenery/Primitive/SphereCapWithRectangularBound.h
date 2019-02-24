// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_SPHERECAPWITHRECTANGULARBOUND_H_
#define SCENERY_PRIMITIVE_SPHERECAPWITHRECTANGULARBOUND_H_

#include <vector>
#include <string>
#include "Scenery/geometry/SphericalCapRayIntersectionEquation.h"
#include "Scenery/geometry/RectangularPrismZ.h"
#include "Scenery/geometry/SurfaceWithOuterPrismBound.h"

namespace relleums {

class SphereCapWithRectangularBound :public SurfaceWithOuterPrismBound {
 protected:
    RectangularPrismZ rect_bounds;
    double curvature_radius;

 public:
    void set_curvature_radius_and_x_y_width(
        const double _curvature_radius,
        const double x_width,
        const double y_width);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;
    double get_focal_length()const;

 private:
    void restrict_cap_radius_to_curvature_radius();
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_SPHERECAPWITHRECTANGULARBOUND_H_
