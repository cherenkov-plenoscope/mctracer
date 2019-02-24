// Copyright 2014 Sebastian A. Mueller
#include "Scenery/primitive/Plane.h"
#include <math.h>
#include <sstream>
using std::string;
using std::vector;
using std::stringstream;

namespace relleums {

Plane::Plane() {}

Plane::Plane(const string name, const Vec3 pos, const Rot3 rot):
    SurfaceEntity(name, pos, rot) {}

void Plane::set_x_y_width(const double x_width, const double y_width) {
    RectBounds.set_x_y_width(x_width, y_width);
    post_initialize_radius_of_enclosing_sphere();
}

void Plane::post_initialize_radius_of_enclosing_sphere() {
    bounding_sphere_radius = hypot(
        RectBounds.get_half_x_width(),
        RectBounds.get_half_y_width());
}

string Plane::str()const {
    stringstream out;
    out << SurfaceEntity::str();
    out << "plane:\n";
    out << "| x width: " << 2.0*RectBounds.get_half_x_width() << "m\n";
    out << "| y width: " << 2.0*RectBounds.get_half_y_width() << "m\n";
    out << "| area:    ";
    out << 4.0*RectBounds.get_half_x_width()*RectBounds.get_half_y_width();
    out << "m^2\n";
    return out.str();
}

#include "Core/Ray.h"
#include "Core/Intersection.h"

void Plane::calculate_intersection_with(
    const Ray* ray,
    vector<Intersection> *intersections
)const {
    XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);
    if (xyPlaneRayEquation.has_causal_solution()) {
        double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
        Vec3 intersection_vector = ray->position_at(v);
        if (RectBounds.is_inside(&intersection_vector)) {
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
