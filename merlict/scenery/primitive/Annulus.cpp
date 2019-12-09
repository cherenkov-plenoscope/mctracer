// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/primitive/Annulus.h"
#include <math.h>
#include <sstream>


namespace merlict {

void Annulus::set_outer_inner_radius(
        const double outer_radius,
        const double inner_radius
) {
    outer_bound.set_radius(outer_radius);
    inner_bound.set_radius(inner_radius);
    post_initialize_radius_of_enclosing_sphere();
}

void Annulus::post_initialize_radius_of_enclosing_sphere() {
    bounding_sphere_radius = outer_bound.get_radius();
}

std::string Annulus::str()const {
    std::stringstream out;
    out << SurfaceEntity::str();
    out << "Annulus:\n";
    out << "| Ri: " << inner_bound.get_radius() << "m, ";
    out << "Ro: " << outer_bound.get_radius() << "m\n";
    out << "| area:   " << get_area() << "m^2\n";
    return out.str();
}

double Annulus::get_area()const {
    return M_PI*(
        outer_bound.get_radius()*outer_bound.get_radius() -
        inner_bound.get_radius()*inner_bound.get_radius());
}

void Annulus::calculate_intersection_with(
    const Ray* ray,
    std::vector<Intersection> *intersections
)const {
    XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);
    if (xyPlaneRayEquation.has_causal_solution()) {
        double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
        Vec3 intersection_vector = ray->position_at(v);
        if (
            outer_bound.is_inside(&intersection_vector) &&
            !inner_bound.is_inside(&intersection_vector)
        ) {
            if (ray->support() != intersection_vector) {
                intersections->emplace_back(
                    std::static_pointer_cast<const SurfaceEntity>(this->shared_from_this()),
                    intersection_vector,
                    xyPlaneRayEquation.get_plane_normal_vector(),
                    v,
                    ray->direction());
            }
        }
    }
}

}  // namespace merlict
