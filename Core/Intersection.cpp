// Copyright 2014 Sebastian A. Mueller
#include "Core/Intersection.h"
#include <sstream>
#include <limits>

Intersection::Intersection() {
    object = &SurfaceEntity::VOID_SURFACE_ENTITY;
    intersection_point = Vec3(
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity());
    surfacenormal_in_intersection_point = Vec3(0.0, 0.0, 1.0);
    distance_of_ray = std::numeric_limits<double>::infinity();
}

Intersection::Intersection(
    const SurfaceEntity* intersectiong_object,
    const Vec3 intersection_vector,
    const Vec3 surfacenormal,
    const double distance_of_ray_support_to_intersection,
    const Vec3 incident_in_obj_sys
):
    intersection_point(intersection_vector),
    surfacenormal_in_intersection_point(surfacenormal),
    object(intersectiong_object),
    distance_of_ray(distance_of_ray_support_to_intersection),
    _from_outside_to_inside(
        ray_is_running_from_outside_to_inside(incident_in_obj_sys))
{}

bool Intersection::does_intersect()const {
    return object != &SurfaceEntity::VOID_SURFACE_ENTITY;
}

const SurfaceEntity * Intersection::get_object()const {
    return object;
}

Vec3 Intersection::position_in_object_frame()const {
    return intersection_point;
}

Vec3 Intersection::position_in_root_frame()const {
    return object->
        frame2world()->get_transformed_position(intersection_point);
}

Vec3 Intersection::surface_normal_in_object_frame()const {
    return surfacenormal_in_intersection_point;
}

Vec3 Intersection::surface_normal_in_root_frame()const {
    return object->frame2world()->
        get_transformed_orientation(surfacenormal_in_intersection_point);
}

double Intersection::distance_to_ray_support()const {
    return distance_of_ray;
}

std::string Intersection::str()const {
    std::stringstream out;
    if (does_intersect()) {
        out << object->get_name() << " in ";
        out << position_in_root_frame().str() << " after ";
        out << distance_to_ray_support() << "m";
    } else {
        out << "no intersection with any object";
    }
    out << "\n";
    return out.str();
}

Vec3 Intersection::reflection_direction_in_root_frame(
    Vec3 in_dir_world
)const {
    Vec3 in_dir_obj = object->frame2world()->
        get_transformed_orientation_inverse(in_dir_world);
    surfacenormal_in_intersection_point.mirror(&in_dir_obj);
    return object->frame2world()->get_transformed_orientation(in_dir_obj);
}

#include "Core/Ray.h"

double Intersection::get_facing_reflection_propability(
    const double wavelength
)const {
    return _from_outside_to_inside ?
    (*object->get_outer_reflection_())(wavelength):
    (*object->get_inner_reflection_())(wavelength);
}

double Intersection::get_refractive_index_going_to(
    const double wavelength
)const {
    return _from_outside_to_inside ?
    (*object->get_inner_refraction_())(wavelength):
    (*object->get_outer_refraction_())(wavelength);
}

bool Intersection::going_to_default_refractive_index()const {
    return _from_outside_to_inside ?
    object->get_inner_refraction_() == SurfaceEntity::DEFAULT_REFRACTION:
    object->get_outer_refraction_() == SurfaceEntity::DEFAULT_REFRACTION;
}

double Intersection::get_refractive_index_coming_from(
    const double wavelength
)const {
    return _from_outside_to_inside ?
    (*object->get_outer_refraction_())(wavelength):
    (*object->get_inner_refraction_())(wavelength);
}

double Intersection::get_half_way_depth_coming_from(
    const double wavelength
)const {
    return _from_outside_to_inside ?
    (*object->get_outer_absorption_())(wavelength):
    (*object->get_inner_absorption_())(wavelength);
}

double Intersection::get_half_way_depth_going_to(
    const double wavelength
)const {
    return _from_outside_to_inside ?
    (*object->get_inner_absorption_())(wavelength):
    (*object->get_outer_absorption_())(wavelength);
}

bool Intersection::boundary_layer_is_transparent()const {
    return object->boundary_layer_is_transparent();
}

const Color Intersection::facing_color()const {
    return _from_outside_to_inside ?
        *object->get_outer_color():
        *object->get_inner_color();
}

bool Intersection::ray_is_running_from_outside_to_inside(
    const Vec3& incident_in_obj_sys
)const {
    double projection_of_incident_onto_normal =
        surface_normal_in_object_frame()*incident_in_obj_sys;
    return projection_of_incident_onto_normal < 0.0;
}

bool Intersection::from_outside_to_inside()const {
    return _from_outside_to_inside;
}

Vec3 Intersection::get_normal_in_faceing_surface_system()const {
    return _from_outside_to_inside ?
    surfacenormal_in_intersection_point:
    surfacenormal_in_intersection_point*-1.0;
}

const HomTra3* Intersection::object2world()const {
    return object->frame2world();
}

bool Intersection::operator<(const Intersection& other)const {
    return (
        this->distance_to_ray_support() <
        other.distance_to_ray_support());
}

bool Intersection::compare(const Intersection &one, const Intersection &two) {
    return  one.distance_to_ray_support() < two.distance_to_ray_support();
}
