// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_XYPLANERAYINTERSECTIONEQUATION_H_
#define SCENERY_GEOMETRY_XYPLANERAYINTERSECTIONEQUATION_H_

#include "Core/Ray.h"

class XyPlaneRayIntersectionEquation {
    double ray_parameter_v;
    bool has_valid_intersection;

 public:
    explicit XyPlaneRayIntersectionEquation(const Ray* ray);
    bool has_solution()const;
    bool has_causal_solution()const;
    double get_ray_parameter_for_intersection()const;
    Vec3 get_plane_normal_vector()const;
};
#endif  // SCENERY_GEOMETRY_XYPLANERAYINTERSECTIONEQUATION_H_
