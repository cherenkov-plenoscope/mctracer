// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_ELLIPTICALCAPRAYINTERSECTIONEQUATION_H_
#define SCENERY_GEOMETRY_ELLIPTICALCAPRAYINTERSECTIONEQUATION_H_

#include "TwoSolutionSurfaceRayEquation.h"

namespace merlict {

class EllipticalCapRayIntersectionEquation :
    public TwoSolutionSurfaceRayEquation {
    const double A, B, C, iAA, iBB, iCC;
    double plus_solution;
    double minus_solution;
    bool _has_solutions = false;

 public:
    EllipticalCapRayIntersectionEquation(
        const double _A,
        const double _B,
        const double _C,
        const double _iAA,
        const double _iBB,
        const double _iCC,
        const Ray* ray);
    bool has_solutions()const;
    double get_minus_solution()const;
    double get_plus_solution()const;
    Vec3 get_surface_normal_given_intersection_vector(
        const Vec3* intersec)const;
};

}  // namespace relleums

#endif  // SCENERY_GEOMETRY_ELLIPTICALCAPRAYINTERSECTIONEQUATION_H_
