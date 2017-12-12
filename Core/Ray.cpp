// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Core/Ray.h"

using std::string;
using std::stringstream;

Ray::Ray() {}

Ray::Ray(const Vec3 support, const Vec3 direction) {
    set_support_and_direction(support, direction);
}

void Ray::set_support_and_direction(const Vec3 support, const Vec3 direction) {
    set_support(support);
    set_direction(direction);
}

void Ray::set_support(const Vec3 sup) {
    support = sup;
}

void Ray::set_direction(const Vec3 dir) {
    direction = dir;
    direction.normalize();
}

string Ray::str()const {
    stringstream out;
    out << "support: " << support.str() << ", direction: " << direction.str();
    return out.str();
}

Vec3 Ray::get_pos_at(const double scalar)const {
    return support + direction*scalar;
}

Vec3 Ray::get_support()const {
    return support;
}

Vec3 Ray::get_direction()const {
    return direction;
}

void Ray::transform(const HomTra3 *T) {
    support = T->get_transformed_position(support);
    direction = T->get_transformed_orientation(direction);
}

void Ray::transform_inverse(const HomTra3 *T) {
    support = T->get_transformed_position_inverse(support);
    direction = T->get_transformed_orientation_inverse(direction);
}

double Ray::get_parameter_on_ray_for_closest_distance_to_point(
    const Vec3 &point
)const {
    // We create a plane orthogonal to this ray and containing the point
    // plane equation:
    //  d = x*a + y*b + z*c
    //
    // We set the normal vector n of the plane to the ray's direction vector:
    //  a=direction.x b=direction.y c=direction.z
    //
    // Now we insert the support vector of the frame into the plane eqaution:
    //  d = point.x*dirx + point.y*diry + point.z*dirz
    const double d = direction*point;

    // Insert the ray into plane equation and solve for the ray parameter.
    // The ray's direction is normalized, therefore: (direction * direction)=1
    return d - support*direction;
}

double Ray::closest_distance_to_point(const Vec3 &point)const {
    const double a = get_parameter_on_ray_for_closest_distance_to_point(point);
    const Vec3 q = get_pos_at(a);
    const Vec3 shortest_connection = point - q;
    return shortest_connection.norm();
}
