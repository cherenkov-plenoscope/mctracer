// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/primitive/SphereCapWithHexagonalBound.h"
#include <math.h>
#include <sstream>
using std::string;
using std::vector;
using std::stringstream;

namespace relleums {

void SphereCapWithHexagonalBound::set_curvature_radius_and_outer_hex_radius(
    const double curvature_radius,
    const double outer_hex_radius
) {
    this->curvature_radius = curvature_radius;
    this->outer_hex_radius = outer_hex_radius;
    restrict_outer_hex_radius_to_curvature_radius();
    hexBounds.set_outer_radius(outer_hex_radius);
    post_initialize_radius_of_enclosing_sphere();
}

double SphereCapWithHexagonalBound::get_focal_length()const {
    return curvature_radius*0.5;
}

void SphereCapWithHexagonalBound::post_initialize_radius_of_enclosing_sphere() {
    double sphere_center_to_edge_in_z = sqrt(
        curvature_radius*curvature_radius -
        outer_hex_radius*outer_hex_radius);

    bounding_sphere_radius = hypot(
        outer_hex_radius,
        curvature_radius - sphere_center_to_edge_in_z);
}

void SphereCapWithHexagonalBound::
    restrict_outer_hex_radius_to_curvature_radius() {
    if (outer_hex_radius > curvature_radius)
        outer_hex_radius = curvature_radius;
}

std::string SphereCapWithHexagonalBound::str()const {
    std::stringstream out;
    out << SurfaceEntity::str();
    out << "sphere cap with hexagonal bound:\n";
    out << "| focal length: " << get_focal_length() << "m\n";
    out << "| curvature radius: " << curvature_radius << "m\n";
    out << "| outer hex radius: " << outer_hex_radius << "m\n";
    out << "| f/D max: " << get_focal_length()/(2.0*outer_hex_radius) << "\n";
    out << "| frontal projected area: ";
    out << hexBounds.get_area() << "m^2\n";
    return out.str();
}

void SphereCapWithHexagonalBound::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    SphericalCapRayIntersectionEquation sphereCapRayEq(curvature_radius, ray);
    if (sphereCapRayEq.has_solutions()) {
        add_causeal_intersection(
            &sphereCapRayEq,
            &hexBounds,
            ray,
            intersections);
    }
}

}  // namespace relleums
