// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/RayAndFrame.h"
#include "merlict/scenery/primitive/Sphere.h"
using namespace relleums;
namespace raf = RayAndFrame;



TEST_CASE("RayAndFrameBoundingSphereTest: frame_has_bounding_sphere", "[mctracer]") {
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);
    CHECK(sphere.get_bounding_sphere_radius() == 1.0);
}

TEST_CASE("RayAndFrameBoundingSphereTest: support_inside_bounding_sphere", "[mctracer]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |      ______\|
    //              |    (0,0,0) /|
    //              \             /
    //                \__     __/
    //                   -----
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);

    Ray ray(VEC3_ORIGIN, VEC3_UNIT_X);
    CHECK(raf::ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: support_outside_bounding_sphere", "[mctracer]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |    (0,0,0)  |         _________\                        //
    //              |    r=1      |       (+2,0,0)   /
    //              \             /
    //                \__     __/
    //                   -----
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);

    Ray ray(Vec3(2.0, 0.0, 0.0), VEC3_UNIT_X);
    CHECK(!raf::ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: frontal_hit", "[mctracer]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //    _________\|    (0,0,0)  |
    //  (-2,0,0)   /|    r=1      |
    //              \             /
    //                \__     __/
    //                   -----
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);

    Ray ray(Vec3(-2.0, 0.0, 0.0), VEC3_UNIT_X);
    CHECK(raf::ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: no_hit", "[mctracer]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |    (0,0,0)  |         _________\                        //
    //              |    r=1      |       (+2,0,0)   /
    //              \             /
    //                \__     __/
    //                   -----
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);

    Ray ray(Vec3(+2.0, 0.0, 0.0), VEC3_UNIT_X);
    CHECK(!raf::ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: hit_inside_bounding_sphere", "[mctracer]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |      ______\|
    //              |    (0,0,0) /|
    //              \             /
    //                \__     __/
    //                   -----
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(1.0);

    Ray ray(VEC3_ORIGIN, VEC3_UNIT_X);
    CHECK(raf::ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: frontal_hits", "[mctracer]") {
    //    _________\                                                          //
    // (-5,+2.0,0) /
    //    .           __-----__
    //    .          /         \                                              //
    //    .        /             \                                            //
    //    ________\|    (0,0,0)  |
    // (-5,0,0)   /|    r=1      |
    //    .        \             /
    //    .          \__     __/
    //    .             -----
    //    _________\                                                          //
    // (-5,-2.0,0) /

    const double sphere_radius = 1.0;
    Sphere sphere("sphere", VEC3_ORIGIN, ROT3_UNITY);
    sphere.set_radius(sphere_radius);

    for (double y = -2.0; y < 2.0; y+=0.002) {
        Ray ray(Vec3(-5.0, y, 0.0), VEC3_UNIT_X);
        if (fabs(y) > sphere_radius)
            CHECK(!raf::ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
        else
            CHECK(raf::ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
    }
}



TEST_CASE("RayAndFrameTest: transform_into_unit_frame", "[mctracer]") {
    Frame frame;
    frame.set_name_pos_rot("frame", VEC3_ORIGIN, ROT3_UNITY);
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(VEC3_ORIGIN, VEC3_UNIT_Z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray,
        &frame);

    CHECK(VEC3_ORIGIN == ray_t.support());
    CHECK(VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_into_translated_frame", "[mctracer]") {
    Frame frame;
    frame.set_name_pos_rot("frame", Vec3(0.0, 0.0, 13.37), ROT3_UNITY);
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3(0.0, 4.2, 0.0), VEC3_UNIT_Z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray,
        &frame);

    CHECK(Vec3(0.0, 4.2, -13.37) == ray_t.support());
    CHECK(VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_ray_in_z_into_frame_rotated_in_z", "[mctracer]") {
    Frame frame;
    frame.set_name_pos_rot("frame", VEC3_ORIGIN, Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(VEC3_ORIGIN, VEC3_UNIT_Z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray,
        &frame);

    CHECK(VEC3_ORIGIN == ray_t.support());
    CHECK(VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_ray_into_rotated_frame", "[mctracer]") {
    Frame frame;
    frame.set_name_pos_rot("frame", VEC3_ORIGIN, Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(VEC3_ORIGIN, VEC3_UNIT_X);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray,
        &frame);

    CHECK(VEC3_ORIGIN == ray_t.support());
    CHECK(VEC3_UNIT_Y == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: causal_intersection", "[mctracer]") {
    Frame spheres_in_a_row;
    spheres_in_a_row.set_name_pos_rot("spheres", VEC3_ORIGIN, ROT3_UNITY);

    Sphere* s0 = spheres_in_a_row.append<Sphere>();
    s0->set_name_pos_rot("s0", Vec3(0.0, 0.0, 0.0), ROT3_UNITY);
    s0->set_radius(1.0);
    Sphere* s1 = spheres_in_a_row.append<Sphere>();
    s1->set_name_pos_rot("s1", Vec3(0.0, 3.0, 0.0), ROT3_UNITY);
    s1->set_radius(1.0);
    Sphere* s2 = spheres_in_a_row.append<Sphere>();
    s2->set_name_pos_rot("s2", Vec3(0.0, 6.0, 0.0), ROT3_UNITY);
    s2->set_radius(1.0);
    Sphere* s3 = spheres_in_a_row.append<Sphere>();
    s3->set_name_pos_rot("s3", Vec3(0.0, 9.0, 0.0), ROT3_UNITY);
    s3->set_radius(1.0);
    Sphere* s4 = spheres_in_a_row.append<Sphere>();
    s4->set_name_pos_rot("s4", Vec3(0.0, 12.0, 0.0), ROT3_UNITY);
    s4->set_radius(1.0);

    spheres_in_a_row.init_tree_based_on_mother_child_relations();
}
