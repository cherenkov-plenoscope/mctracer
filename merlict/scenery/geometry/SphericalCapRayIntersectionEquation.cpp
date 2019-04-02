// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/geometry/SphericalCapRayIntersectionEquation.h"

namespace merlict {

SphericalCapRayIntersectionEquation::SphericalCapRayIntersectionEquation(
    const double _radius,
    const Ray* ray
):
    radius(_radius) {
    // sphere intersection with ray
    // z(x,y) = R - sqrt(R^2 -x^2 -y^2) |- R, *(-1)
    // R - z  = sqrt(R^2 -x^2 -y^2)     |pow2
    // (R-z)^2= R^2 - x^2 - y^2         |+x^2 +y^2
    // R^2 -2Rz +z^2 +y^2 +x^2 = R^2    |-R^2
    // x^2 + y^2 + z^2 -2Rz = 0         | x=sx+v*dx, y=sy+v*dy, z=sz+v*dz
    // v^2*(d*d) + v*2(s*d -R*dz) +s*s -2*R*sz = 0
    // v^2 + v 2(s*d - R*dz)/(d*d) + (s*s - 2*R*sz)/(d*d) = 0
    const Vec3 sup = ray->support();
    const Vec3 dir = ray->direction();

    const double dir_times_dir = dir*dir;
    const double sup_times_sup = sup*sup;
    const double sup_times_dir = sup*dir;
    const double R_times_dir_z = radius*dir.z;
    const double R_times_sup_z = radius*sup.z;

    const double P = 2.0 * (sup_times_dir - R_times_dir_z) / dir_times_dir;
    const double Q = (sup_times_sup - 2.0*R_times_sup_z) / dir_times_dir;

    QuadraticEquation RayParamForSphereCapIntersecEq(P, Q);

    if (RayParamForSphereCapIntersecEq.has_valid_solutions()) {
        _has_solutions = true;
        plus_solution = RayParamForSphereCapIntersecEq.plus_solution();
        minus_solution = RayParamForSphereCapIntersecEq.minus_solution();
    }
}

bool SphericalCapRayIntersectionEquation::has_solutions()const {
    return _has_solutions;
}

double SphericalCapRayIntersectionEquation::get_minus_solution()const {
    return minus_solution;
}

double SphericalCapRayIntersectionEquation::get_plus_solution()const {
    return plus_solution;
}

Vec3 SphericalCapRayIntersectionEquation::
    get_surface_normal_given_intersection_vector(
    const Vec3* intersec
)const {
    Vec3 normal(Vec3(0.0, 0.0, radius)- *intersec);
    normal = normal/normal.norm();
    return normal;
}

}  // namespace merlict
