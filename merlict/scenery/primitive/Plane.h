// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_PLANE_H_
#define SCENERY_PRIMITIVE_PLANE_H_

#include <string>
#include <vector>
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"
#include "merlict/scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "merlict/scenery/geometry/RectangularPrismZ.h"

namespace merlict {

class Plane :public SurfaceEntity {
 protected:
    RectangularPrismZ RectBounds;

 public:
    Plane();
    Plane(const std::string name, const Vec3 pos, const Rot3 rot);
    void set_x_y_width(
        const double x_width,
        const double y_width);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace merlict

#endif  // SCENERY_PRIMITIVE_PLANE_H_
