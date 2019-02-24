// Copyright 2014 Sebastian A. Mueller
#include "Scenery/geometry/ZaxisCylinderRayIntersectionEquation.h"

namespace relleums {

ZaxisCylinderRayIntersectionEquation::ZaxisCylinderRayIntersectionEquation(
    const double radius,
    const Ray* ray
) {
    this->radius = radius;
    this->ray = ray;
    // ray := s+l*d, s = (sx, sy, sz)^T, d = (dx, dy, dz)^T
    //
    // r = sqrt(x^2 + y^2)                                              (1)
    //
    // r = sqrt( (sx+l*dy)^2 + (sy+l*dy)^2 )                            (2)
    //
    // r = sqrt( l^2*(dx^2 + dy^2) + l*2*(sx*dx + sy*dy) +sx^2 +sy^2)   (3)
    //
    // 0 = l^2 +
    //     l*2*(sxdx + sydy)/(dx^2 + dy^2) +
    //     (sx^2 +sy^2 -r^2)/(dx^2 + dy^2)

    const double dx2 = ray->direction().x*ray->direction().x;
    const double dy2 = ray->direction().y*ray->direction().y;

    const double sxdx = ray->support().x*ray->direction().x;
    const double sydy = ray->support().y*ray->direction().y;

    const double sx2 = ray->support().x*ray->support().x;
    const double sy2 = ray->support().y*ray->support().y;

    const double r2 = radius*radius;
    const double dnom = dx2 + dy2;

    const double p = 2.0*(sxdx + sydy) / dnom;
    const double q = (sx2 + sy2 -r2) / dnom;

    QuadraticEquation rayParamCylinderIntersecEquation(p, q);

    if (rayParamCylinderIntersecEquation.has_valid_solutions()) {
        has_intersection = true;

        plus_solution = rayParamCylinderIntersecEquation.plus_solution();
        minus_solution = rayParamCylinderIntersecEquation.minus_solution();

        if (one_intersec_behind_and_one_in_front_ray_sup())
            choose_plus_solution();

        if (both_intersects_in_front_ray_sup())
            choose_minus_solution();
    }
}

bool ZaxisCylinderRayIntersectionEquation::
    one_intersec_behind_and_one_in_front_ray_sup() {
    return minus_solution < 0.0 && plus_solution >= 0.0;
}

bool ZaxisCylinderRayIntersectionEquation::both_intersects_in_front_ray_sup() {
    return plus_solution >= 0.0 && minus_solution >= 0.0;
}

void ZaxisCylinderRayIntersectionEquation::choose_minus_solution() {
    has_causal_intersection = true;
    causal_ray_parameter = minus_solution;
}

void ZaxisCylinderRayIntersectionEquation::choose_plus_solution() {
    has_causal_intersection = true;
    causal_ray_parameter = plus_solution;
}

bool ZaxisCylinderRayIntersectionEquation::has_causal_solution()const {
    return has_causal_intersection;
}

double ZaxisCylinderRayIntersectionEquation::
    get_ray_parameter_for_intersection()const {
    return causal_ray_parameter;
}

bool ZaxisCylinderRayIntersectionEquation::has_solution()const {
    return has_intersection;
}

}  // namespace relleums