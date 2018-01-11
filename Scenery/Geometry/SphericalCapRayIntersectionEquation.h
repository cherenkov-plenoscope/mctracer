// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_SPHERICALCAPRAYINTERSECTIONEQUATION_H_
#define SCENERY_GEOMETRY_SPHERICALCAPRAYINTERSECTIONEQUATION_H_

#include "TwoSolutionSurfaceRayEquation.h"

class SphericalCapRayIntersectionEquation :
    public TwoSolutionSurfaceRayEquation {
    double radius;
    double plus_solution;
    double minus_solution;
    bool _has_solutions = false;

 public:
    SphericalCapRayIntersectionEquation(const double radius, const Ray* ray);
    bool has_solutions()const;
    double get_minus_solution()const;
    double get_plus_solution()const;
    Vec3 get_surface_normal_given_intersection_vector(
        const Vec3* intersec)const;
};

#endif  // SCENERY_GEOMETRY_SPHERICALCAPRAYINTERSECTIONEQUATION_H_
