// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_CYLINDER_H_
#define SCENERY_PRIMITIVE_CYLINDER_H_
#include <stdexcept>
#include <vector>
#include <string>
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"
#include "merlict/scenery/geometry/ZaxisCylinderRayIntersectionEquation.h"

namespace merlict {

class Cylinder :public SurfaceEntity {
 protected:
    double Radius;
    double Length;

 public:
    Cylinder();
    Cylinder(const std::string name, const Vec3 pos, const Rot3 rot);
    void set_cylinder(
        const double radius,
        const Vec3 start_pos,
        const Vec3 end_pos);
    void set_radius_and_length(const double radius, const double length);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    void set_cylinder_length(const double Length);
    void set_cylinder_length(const Vec3 start_pos, const Vec3 end_pos);
    void set_cylinder_radius(const double new_cylinder_radius);
    void post_initialize_radius_of_enclosing_sphere();
    void assert_start_and_end_point_are_distinct(
        const Vec3 start_pos, const Vec3 end_pos)const;
    void set_position_and_orientation(
        const Vec3 start_pos,
        const Vec3 end_pos);
    bool is_in_cylinders_z_bounds(const Vec3* vec)const;
    Vec3 get_surface_normal_for_intersection_vec(const Vec3* vec)const;
    Rot3 calculate_new_rotation_in_mother(const Vec3 rotsym_axis)const;

 public:
    class BadRadius :public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class BadStartEndPoints :public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };

    class BadLength :public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };
};

}  // namespace merlict

#endif  // SCENERY_PRIMITIVE_CYLINDER_H_
