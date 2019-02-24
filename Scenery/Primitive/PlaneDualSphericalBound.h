// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_PLANEDUALSPHERICALBOUND_H_
#define SCENERY_PRIMITIVE_PLANEDUALSPHERICALBOUND_H_

#include <vector>
#include <string>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "Scenery/geometry/DualSphericalPrismZ.h"

namespace relleums {

class PlaneDualSphericalBound :public SurfaceEntity{
 protected:
    DualSphericalPrismZ dual_sphere_bounds;

 public:
    void set_x_hight_and_y_width(
        const double x_width,
        const double y_width);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    void post_initialize_radius_of_enclosing_sphere();
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_PLANEDUALSPHERICALBOUND_H_
