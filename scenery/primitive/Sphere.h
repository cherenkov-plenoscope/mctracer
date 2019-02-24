// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_SPHERE_H_
#define SCENERY_PRIMITIVE_SPHERE_H_

#include <string>
#include <vector>
#include <stdexcept>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "scenery/geometry/QuadraticEquation.h"

namespace relleums {

class Sphere :public SurfaceEntity {
 protected:
    double radius;

 public:
    Sphere();
    Sphere(const std::string name, const Vec3 pos, const Rot3 rot);
    void set_radius(double nradius);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    bool facing_sphere_from_outside_given_p_m(
        const double v_Plus,
        const double v_Minus)const;
    bool facing_away_from_outside_given_p_m(
        const double v_Plus,
        const double v_Minus)const;
    void add_sphere_intersection_for_ray_parameter(
        const Ray* ray,
        const double ray_parameter,
        std::vector<Intersection> *intersections)const;
    QuadraticEquation get_ray_parameter_equation_for_intersections_with_sphere(
        const Ray* ray)const;

 public:
    class BadRadius :public std::invalid_argument{
        using invalid_argument::invalid_argument;
    };
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_SPHERE_H_
