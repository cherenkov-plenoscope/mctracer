// Copyright 2014 Sebastian A. Mueller
#include "Scenery/primitive/SphereCapWithCylinderBound.h"
#include <math.h>
#include <sstream>
using std::string;
using std::vector;
using std::stringstream;

namespace relleums {

void SphereCapWithCylinderBound::set_curvature_radius_and_outer_radius(
    const double _curvature_radius,
    const double cap_radius
) {
    curvature_radius = _curvature_radius;
    this->cap_radius = cap_radius;
    restrict_cap_radius_to_curvature_radius();
    CylBounds.set_radius(cap_radius);
    post_initialize_radius_of_enclosing_sphere();
}

double SphereCapWithCylinderBound::get_focal_length()const {
    return curvature_radius*0.5;
}

void SphereCapWithCylinderBound::post_initialize_radius_of_enclosing_sphere() {
    double sphere_center_to_edge_in_z =
        sqrt(curvature_radius*curvature_radius - cap_radius*cap_radius);
    bounding_sphere_radius = hypot(
        cap_radius,
        curvature_radius - sphere_center_to_edge_in_z);
}

void SphereCapWithCylinderBound::restrict_cap_radius_to_curvature_radius() {
    if (cap_radius > curvature_radius)
        cap_radius = curvature_radius;
}

std::string SphereCapWithCylinderBound::str()const {
    std::stringstream out;
    out << SurfaceEntity::str();
    out << "SphereCapWithCylinderBound:\n";
    out << "| focal length: " << get_focal_length() << "m\n";
    out << "| curvature radius: " << curvature_radius << "m\n";
    out << "| cap radius: " << cap_radius << "m\n";
    out << "| f/D: " << get_focal_length()/(2.0*cap_radius) << "\n";
    return out.str();
}

void SphereCapWithCylinderBound::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    SphericalCapRayIntersectionEquation sphereCapRayEq(curvature_radius, ray);
    if (sphereCapRayEq.has_solutions()) {
        add_causeal_intersection(
            &sphereCapRayEq,
            &CylBounds,
            ray,
            intersections);
    }
}

}  // namespace relleums
