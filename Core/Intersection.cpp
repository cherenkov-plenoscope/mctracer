// Copyright 2014 Sebastian A. Mueller
#include "Core/Intersection.h"
#include <sstream>
#include <limits>

Intersection::Intersection() {
    object = &SurfaceEntity::void_object;
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
    return object != &SurfaceEntity::void_object;
}

const SurfaceEntity * Intersection::get_object()const {
    return object;
}

Vec3 Intersection::get_intersection_vector_in_object_system()const {
    return intersection_point;
}

Vec3 Intersection::get_intersection_vector_in_world_system()const {
    return object->
        frame2world()->get_transformed_position(intersection_point);
}

Vec3 Intersection::get_surface_normal_in_object_system()const {
    return surfacenormal_in_intersection_point;
}

Vec3 Intersection::get_surface_normal_in_world_system()const {
    return object->frame2world()->
        get_transformed_orientation(surfacenormal_in_intersection_point);
}

double Intersection::get_intersection_distance()const {
    return distance_of_ray;
}

std::string Intersection::str()const {
    std::stringstream out;
    if (does_intersect()) {
        out << object->get_name() << " in ";
        out << get_intersection_vector_in_world_system().str() << " after ";
        out << get_intersection_distance() << "m";
    } else {
        out << "no intersection with any object";
    }
    out << "\n";
    return out.str();
}

void Intersection::get_reflection_direction_in_object_system(
    Vec3* vec
)const {
    surfacenormal_in_intersection_point.mirror(vec);
}

Vec3 Intersection::get_reflection_direction_in_world_system(
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
    object->get_inner_refraction_() == SurfaceEntity::default_refr:
    object->get_outer_refraction_() == SurfaceEntity::default_refr;
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

const Color Intersection::get_facing_color()const {
    return _from_outside_to_inside ?
        *object->get_outer_color():
        *object->get_inner_color();
}

bool Intersection::ray_is_running_from_outside_to_inside(
    const Vec3 incident_in_obj_sys
)const {
    double projection_of_incident_onto_normal =
        get_surface_normal_in_object_system()*incident_in_obj_sys;

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
        this->get_intersection_distance() <
        other.get_intersection_distance());
}

bool Intersection::compare(const Intersection &one, const Intersection &two) {
    return  one.get_intersection_distance() < two.get_intersection_distance();
}
