// Copyright 2014 Sebastian A. Mueller
#include "merlict/Intersection.h"
#include <sstream>
#include <limits>

namespace merlict {

Intersection::Intersection() {
    __object = std::static_pointer_cast<const SurfaceEntity>(VOID_SURFACE_ENTITY.shared_from_this());
    position = Vec3(
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity());
    surface_normal = Vec3(0.0, 0.0, 1.0);
    distance_of_ray = std::numeric_limits<double>::infinity();
}

Intersection::Intersection(
    std::shared_ptr<const SurfaceEntity> intersectiong_object,
    const Vec3 _position,
    const Vec3 _surface_normal,
    const double distance_of_ray_support_to_intersection,
    const Vec3 incident_in_obj_sys
):
    position(_position),
    surface_normal(_surface_normal),
    __object(intersectiong_object),
    distance_of_ray(distance_of_ray_support_to_intersection),
    __from_outside_to_inside(
        ray_runs_from_outside_to_inside(incident_in_obj_sys))
{}

bool Intersection::does_intersect()const {
    return __object.get() != &VOID_SURFACE_ENTITY;
}

std::shared_ptr<const SurfaceEntity> Intersection::object()const {
    return __object;
}

Vec3 Intersection::position_in_object_frame()const {
    return position;
}

Vec3 Intersection::position_in_root_frame()const {
    return __object->frame2world()->position(position);
}

Vec3 Intersection::surface_normal_in_object_frame()const {
    return surface_normal;
}

Vec3 Intersection::surface_normal_in_root_frame()const {
    return __object->frame2world()->orientation(surface_normal);
}

double Intersection::distance_to_ray_support()const {
    return distance_of_ray;
}

std::string Intersection::str()const {
    std::stringstream out;
    if (does_intersect()) {
        out << __object->get_name() << " in ";
        out << position_in_root_frame().str() << " after ";
        out << distance_to_ray_support() << "m";
    } else {
        out << "no intersection with any __object";
    }
    out << "\n";
    return out.str();
}

Vec3 Intersection::reflection_direction_in_root_frame(Vec3 in_dir_world)const {
    Vec3 in_dir_obj = __object->frame2world()->
        orientation_inverse(in_dir_world);
    surface_normal.mirror(&in_dir_obj);
    return __object->frame2world()->orientation(in_dir_obj);
}

#include "merlict/Ray.h"

double Intersection::facing_reflection_propability(
    const double wavelength
)const {
    return __from_outside_to_inside ?
        __object->outer_reflection->evaluate(wavelength):
        __object->inner_reflection->evaluate(wavelength);
}

double Intersection::refractive_index_going_to(const double wavelength)const {
    return __from_outside_to_inside ?
        __object->inner_refraction->evaluate(wavelength):
        __object->outer_refraction->evaluate(wavelength);
}

bool Intersection::going_to_default_refractive_index()const {
    return __from_outside_to_inside ?
        __object->inner_refraction == SurfaceEntity::DEFAULT_REFRACTION:
        __object->outer_refraction == SurfaceEntity::DEFAULT_REFRACTION;
}

double Intersection::refractive_index_coming_from(
    const double wavelength
)const {
    return __from_outside_to_inside ?
        __object->outer_refraction->evaluate(wavelength):
        __object->inner_refraction->evaluate(wavelength);
}

double Intersection::half_way_depth_coming_from(const double wavelength)const {
    return __from_outside_to_inside ?
        __object->outer_absorption->evaluate(wavelength):
        __object->inner_absorption->evaluate(wavelength);
}

double Intersection::half_way_depth_going_to(const double wavelength)const {
    return __from_outside_to_inside ?
        __object->inner_absorption->evaluate(wavelength):
        __object->outer_absorption->evaluate(wavelength);
}

bool Intersection::boundary_layer_is_transparent()const {
    return __object->boundary_layer_is_transparent();
}

const Color Intersection::facing_color()const {
    return __from_outside_to_inside ?
        *__object->outer_color :
        *__object->inner_color;
}

bool Intersection::ray_runs_from_outside_to_inside(
    const Vec3& incident_in_obj_sys
)const {
    double projection_of_incident_onto_normal =
        surface_normal_in_object_frame()*incident_in_obj_sys;
    return projection_of_incident_onto_normal < 0.0;
}

bool Intersection::from_outside_to_inside()const {
    return __from_outside_to_inside;
}

Vec3 Intersection::surface_normal_of_facing_surface_in_object_frame()const {
    return __from_outside_to_inside ? surface_normal : surface_normal*-1.0;
}

const HomTra3* Intersection::object2root()const {
    return __object->frame2world();
}

bool Intersection::operator<(const Intersection& other)const {
    return this->distance_to_ray_support() < other.distance_to_ray_support();
}

bool Intersection::compare(const Intersection &one, const Intersection &two) {
    return one.distance_to_ray_support() < two.distance_to_ray_support();
}

}  // namespace merlict
