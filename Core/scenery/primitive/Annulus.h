// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_ANNULUS_H_
#define SCENERY_PRIMITIVE_ANNULUS_H_

#include <string>
#include <vector>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Core/scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "Core/scenery/geometry/CylinderPrismZ.h"

namespace relleums {

class Annulus :public SurfaceEntity{
 protected:
    CylinderPrismZ outer_bound;
    CylinderPrismZ inner_bound;

 public:
    void set_outer_inner_radius(
        const double outer_radius,
        const double inner_radius);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    double get_area()const;
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_ANNULUS_H_
