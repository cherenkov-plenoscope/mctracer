// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_DISC_H_
#define SCENERY_PRIMITIVE_DISC_H_

#include <vector>
#include <string>
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"
#include "merlict/scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "merlict/scenery/geometry/CylinderPrismZ.h"

namespace relleums {

class Disc :public SurfaceEntity{
 protected:
    CylinderPrismZ cylinder_bounds;

 public:
    void set_radius(const double radius);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    double get_area()const;
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_DISC_H_
