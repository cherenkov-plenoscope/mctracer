// Copyright 2014 Sebastian A. Mueller
#include "scenery/geometry/EllipticalCapRayIntersectionEquation.h"
#include <math.h>

namespace relleums {

EllipticalCapRayIntersectionEquation::EllipticalCapRayIntersectionEquation(
    const double _A, const double _B,  const double _C,
    const double _iAA, const double _iBB,  const double _iCC,
    const Ray* ray
) :
    A(_A),
    B(_B),
    C(_C),
    iAA(_iAA),
    iBB(_iBB),
    iCC(_iCC) {
// ellipsiod is given by f(x,y,z) = 0 with
// f(x,y,z) = x^2/A^2 + y^2/B^2 + (z-C)^2/C^2 - 1
//
// So this is a general tri-axial ellipsoid with its center
// in (0,0,c). Thus f(0,0,0) is exactly zero, as for all the
// other mirrors as well.
//
// The intersections of this ellipsoid with a general ray given by
// ray = base + dir * t      or short:  b + d * t
// can be expressed as:
//
// 0=((bx+t*dx)^2)/(A^2) +((by+t*dy)^2)/(B^2) +((bz+t*dz-C)^2)/(C^2)
//
// solve for t:
//
// 0=t^2*()
//      p/m stands for plus/minus
//
// We only want to define a "long" and "short" focal length, so in
// our case we define:
//  A = 2*short_focal_length
//  B = 2*long_focal_length
//  c = (A+B)/2   just the mean of the two ...
// since I don't have any better idea.

    const double dx = ray->direction().x;
    const double dy = ray->direction().y;
    const double dz = ray->direction().z;
    const double bx = ray->support().x;
    const double by = ray->support().y;
    const double bz = ray->support().z;

    const double a = (dx*dx)*iAA + (dy*dy)*iBB +  (dz*dz)*iCC;
    const double b = 2.0*( (bx*dx)*iAA + (by*dy)*iBB + dz*(bz-C)*iCC);
    const double c = (bx*bx)*iAA + (by*by)*iBB +  (bz*bz-2.0*bz*C)*iCC;

    const double p = b/a;
    const double q = c/a;

    QuadraticEquation RayParamForEllipticalCapIntersecEq(p, q);

    if (RayParamForEllipticalCapIntersecEq.has_valid_solutions()) {
        _has_solutions = true;
        plus_solution = RayParamForEllipticalCapIntersecEq.plus_solution();
        minus_solution = RayParamForEllipticalCapIntersecEq.minus_solution();
    }
}

bool EllipticalCapRayIntersectionEquation::has_solutions()const {
    return _has_solutions;
}

double EllipticalCapRayIntersectionEquation::get_minus_solution()const {
    return minus_solution;
}

double EllipticalCapRayIntersectionEquation::get_plus_solution()const {
    return plus_solution;
}

Vec3 EllipticalCapRayIntersectionEquation::
    get_surface_normal_given_intersection_vector(const Vec3* intersec)const {
    // surface normal is given as  ( -dz/dx , -dz/dy , 1 )
    //
    // z(x,y) = C*sqrt(1-x^2/A^2-y^2/B^2)+C
    // dz/dx = C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) * (-2*x/A^2)
    // dz/dy = C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) * (-2*y/A^2)
    // normal = ( -dz/dx , -dz/dy , 1 )
    // surface_normal_factor = C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2)
    const double ix = intersec->x;
    const double iy = intersec->y;

    const double surface_normal_factor =
        C * 0.5 * sqrt(1.0 - (ix*ix)*iAA - (iy*iy)*iBB);

    Vec3 surface_normal(
        surface_normal_factor*(-2.0*ix*iAA),
        surface_normal_factor*(-2.0*iy*iAA),
        1.0);

    return surface_normal/surface_normal.norm();
}

}  // namespace relleums
