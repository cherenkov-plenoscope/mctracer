// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_ZAXISCYLINDERRAYINTERSECTIONEQUATION_H_
#define SCENERY_GEOMETRY_ZAXISCYLINDERRAYINTERSECTIONEQUATION_H_

#include "Core/Ray.h"
#include "QuadraticEquation.h"

class ZaxisCylinderRayIntersectionEquation {
    double radius;
    const Ray* ray;
    double plus_solution;
    double minus_solution;
    double causal_ray_parameter;
    bool has_causal_intersection = false;
    bool has_intersection = false;

 public:
    ZaxisCylinderRayIntersectionEquation(const double radius, const Ray* ray);
    bool has_causal_solution()const;
    double get_ray_parameter_for_intersection()const;
    bool has_solution()const;
    bool one_intersec_behind_and_one_in_front_ray_sup();
    bool both_intersects_in_front_ray_sup();

 protected:
    void choose_minus_solution();
    void choose_plus_solution();
};
#endif  // SCENERY_GEOMETRY_ZAXISCYLINDERRAYINTERSECTIONEQUATION_H_
