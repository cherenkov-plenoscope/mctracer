// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_TRIANGLE_H_
#define SCENERY_PRIMITIVE_TRIANGLE_H_

#include <vector>
#include <string>
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"
#include "merlict/Vec2.h"
#include "merlict/scenery/geometry/XyPlaneRayIntersectionEquation.h"

namespace merlict {

class Triangle  :public SurfaceEntity{
    Vec2 A;
    Vec2 B;
    Vec2 C;

 public:
    void set_corners_in_xy_plane(
        const double Ax, const double Ay,
        const double Bx, const double By,
        const double Cx, const double Cy);
    void set_normal_and_3_vertecies(
        const Vec3 nomal,
        Vec3 a,
        Vec3 b,
        Vec3 c);
    std::string str()const;
    void calculate_intersection_with(
        const Ray* ray,
        std::vector<Intersection> *intersections)const;

 private:
    void assert_edge_length_is_non_zero(
        const double edge_length,
        const std::string edge)const;
    void post_initialize_radius_of_enclosing_sphere();
    double sign(
        const Vec3 &P1,
        const Vec2 &P2,
        const Vec2 &P3)const;
    bool is_inside_triangle(const Vec3 &intersec_vec)const;
};

}  // namespace merlict

#endif  // SCENERY_PRIMITIVE_TRIANGLE_H_
