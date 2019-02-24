// Copyright 2014 Sebastian A. Mueller
#include "scenery/primitive/Sphere.h"
#include <math.h>
#include <sstream>
#include "Core/Ray.h"
#include "Core/Intersection.h"

using std::string;
using std::stringstream;
using std::vector;

namespace relleums {

Sphere::Sphere() {}

Sphere::Sphere(const string name, const Vec3 pos, const Rot3 rot):
    SurfaceEntity(name, pos, rot) {}

void Sphere::set_radius(double new_radius) {
    if (new_radius <= 0.0) {
        stringstream info;
        info << "Expected sphere radius to be greater 0.0, but actual ";
        info << "r = " << new_radius << "m.";
        throw BadRadius(info.str());
    }
    radius = new_radius;
    bounding_sphere_radius = new_radius;
}

string Sphere::str()const {
    stringstream out;
    out << SurfaceEntity::str();
    out << "sphere:\n";
    out << "| radius: " << radius << "m\n";
    out << "| area:   " << 4.0*M_PI*radius*radius << "m^2\n";
    return out.str();
}

bool Sphere::facing_sphere_from_outside_given_p_m(
    const double v_Plus,
    const double v_Minus
)const {
    return v_Plus >= 0.0 && v_Minus >= 0.0;
}

bool Sphere::facing_away_from_outside_given_p_m(
    const double v_Plus,
    const double v_Minus
)const {
    return v_Plus < 0.0 && v_Minus < 0.0;
}

QuadraticEquation Sphere::
    get_ray_parameter_equation_for_intersections_with_sphere(
    const Ray* ray
)const {
    //
    //      r = sqrt( x^2 + y^2 + z^2 )
    //      g:  b + v*d
    //      put g into sphere eq
    //      r = sqrt((bx+v*dx)^2 + (by+v*dy)^2 + (bz+v*dz)^2 )
    //      r = sqrt( v^2 (dd) + v 2(bd) + (bb) )
    //      r^2 = v^2 (dd) + v 2(bd) + (bb)
    //      0 = v^2 + v 2(bd/dd) + (bb/dd -r^2)
    //      solve quadrativ eqaution in v
    const double sup_times_dir = ray->support() * ray->direction();
    const double dir_times_dir = ray->direction() * ray->direction();
    const double sup_times_sup = ray->support() * ray->support();
    const double radius_square = radius*radius;
    QuadraticEquation quadratic_eq_in_v(
        2*(sup_times_dir / dir_times_dir),
        sup_times_sup/dir_times_dir - radius_square);
    return quadratic_eq_in_v;
}

void Sphere::add_sphere_intersection_for_ray_parameter(
    const Ray* ray,
    const double ray_parameter,
    vector<Intersection> *intersections
)const {
    Vec3 intersection_point = ray->position_at(ray_parameter);
    Vec3 surface_normal = intersection_point/intersection_point.norm();
    if (ray->support() != intersection_point) {
        intersections->emplace_back(
            this,
            intersection_point,
            surface_normal,
            ray_parameter,
            ray->direction());
    }
}

void Sphere::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    QuadraticEquation rayParamEqForIntersections =
        get_ray_parameter_equation_for_intersections_with_sphere(ray);
    if (rayParamEqForIntersections.has_valid_solutions()) {
        const double vP = rayParamEqForIntersections.plus_solution();
        const double vM = rayParamEqForIntersections.minus_solution();
        if (facing_sphere_from_outside_given_p_m(vP, vM))
            add_sphere_intersection_for_ray_parameter(ray, vM, intersections);
        else if (!facing_away_from_outside_given_p_m(vP, vM))
            add_sphere_intersection_for_ray_parameter(ray, vP, intersections);
    }
}

}  // namespace relleums
