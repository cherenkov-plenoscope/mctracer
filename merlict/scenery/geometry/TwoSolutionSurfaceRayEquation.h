// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_TWOSOLUTIONSURFACERAYEQUATION_H_
#define SCENERY_GEOMETRY_TWOSOLUTIONSURFACERAYEQUATION_H_

#include "merlict/Ray.h"
#include "QuadraticEquation.h"

namespace merlict {

class TwoSolutionSurfaceRayEquation {
 public:
    virtual bool has_solutions()const = 0;
    virtual double get_minus_solution()const = 0;
    virtual double get_plus_solution()const = 0;
    virtual Vec3 get_surface_normal_given_intersection_vector(
        const Vec3* intersec)const = 0;
};

}  // namespace relleums

#endif  // SCENERY_GEOMETRY_TWOSOLUTIONSURFACERAYEQUATION_H_
