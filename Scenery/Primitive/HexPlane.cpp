// Copyright 2014 Sebastian A. Mueller
#include "Scenery/Primitive/HexPlane.h"
#include <sstream>
#include "Core/Ray.h"
#include "Core/Intersection.h"

using std::vector;
using std::string;
using std::stringstream;

namespace relleums {

void HexPlane::set_outer_hex_radius(const double outer_hex_radius) {
    hex_bounds.set_outer_radius(outer_hex_radius);
    post_initialize_radius_of_enclosing_sphere();
}

void HexPlane::post_initialize_radius_of_enclosing_sphere() {
    bounding_sphere_radius = hex_bounds.get_outer_radius();
}

string HexPlane::str()const {
    stringstream out;
    out << SurfaceEntity::str();
    out << "hexagonal plane:\n";
    out << "| outer radius: " << hex_bounds.get_outer_radius() << "m\n";
    out << "| area: "<< hex_bounds.get_area() << "m^2\n";
    return out.str();
}

void HexPlane::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);
    if (xyPlaneRayEquation.has_causal_solution()) {
        double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
        Vec3 intersection_vector = ray->position_at(v);
        if (hex_bounds.is_inside(&intersection_vector)) {
            if (ray->support() != intersection_vector) {
                intersections->emplace_back(
                    this,
                    intersection_vector,
                    xyPlaneRayEquation.get_plane_normal_vector(),
                    v,
                    ray->direction());
            }
        }
    }
}

}  // namespace relleums
