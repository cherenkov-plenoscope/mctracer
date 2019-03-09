// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/RayAndFrame.h"
#include "merlict/scenery/primitive/Sphere.h"
namespace ml = merlict;


TEST_CASE("RayAndFrameBoundingSphereTest: frame_has_bounding_sphere", "[merlict]") {
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);
    CHECK(sphere.get_bounding_sphere_radius() == 1.0);
}

TEST_CASE("RayAndFrameBoundingSphereTest: support_inside_bounding_sphere", "[merlict]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |      ______\|
    //              |    (0,0,0) /|
    //              \             /
    //                \__     __/
    //                   -----
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_X);
    CHECK(ml::RayAndFrame::ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: support_outside_bounding_sphere", "[merlict]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |    (0,0,0)  |         _________\                        //
    //              |    r=1      |       (+2,0,0)   /
    //              \             /
    //                \__     __/
    //                   -----
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);

    ml::Ray ray(ml::Vec3(2.0, 0.0, 0.0), ml::VEC3_UNIT_X);
    CHECK(!ml::RayAndFrame::ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: frontal_hit", "[merlict]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //    _________\|    (0,0,0)  |
    //  (-2,0,0)   /|    r=1      |
    //              \             /
    //                \__     __/
    //                   -----
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);

    ml::Ray ray(ml::Vec3(-2.0, 0.0, 0.0), ml::VEC3_UNIT_X);
    CHECK(ml::RayAndFrame::ray_intersects_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: no_hit", "[merlict]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |    (0,0,0)  |         _________\                        //
    //              |    r=1      |       (+2,0,0)   /
    //              \             /
    //                \__     __/
    //                   -----
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);

    ml::Ray ray(ml::Vec3(+2.0, 0.0, 0.0), ml::VEC3_UNIT_X);
    CHECK(!ml::RayAndFrame::ray_intersects_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: hit_inside_bounding_sphere", "[merlict]") {
    //                 __-----__
    //                /         \                                             //
    //              /             \                                           //
    //              |      ______\|
    //              |    (0,0,0) /|
    //              \             /
    //                \__     __/
    //                   -----
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(1.0);

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_X);
    CHECK(ml::RayAndFrame::ray_intersects_frames_bounding_sphere(&ray, &sphere));
}

TEST_CASE("RayAndFrameBoundingSphereTest: frontal_hits", "[merlict]") {
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
    ml::Sphere sphere("sphere", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    sphere.set_radius(sphere_radius);

    for (double y = -2.0; y < 2.0; y+=0.002) {
        ml::Ray ray(ml::Vec3(-5.0, y, 0.0), ml::VEC3_UNIT_X);
        if (fabs(y) > sphere_radius)
            CHECK(!ml::RayAndFrame::ray_intersects_frames_bounding_sphere(&ray, &sphere));
        else
            CHECK(ml::RayAndFrame::ray_intersects_frames_bounding_sphere(&ray, &sphere));
    }
}



TEST_CASE("RayAndFrameTest: transform_into_unit_frame", "[merlict]") {
    ml::Frame frame;
    frame.set_name_pos_rot("frame", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    frame.init_tree_based_on_mother_child_relations();

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z);

    ml::Ray ray_t = ml::RayAndFrame::ray_with_respect_to_frame(
        &ray,
        &frame);

    CHECK(ml::VEC3_ORIGIN == ray_t.support());
    CHECK(ml::VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_into_translated_frame", "[merlict]") {
    ml::Frame frame;
    frame.set_name_pos_rot("frame", ml::Vec3(0.0, 0.0, 13.37), ml::ROT3_UNITY);
    frame.init_tree_based_on_mother_child_relations();

    ml::Ray ray(ml::Vec3(0.0, 4.2, 0.0), ml::VEC3_UNIT_Z);

    ml::Ray ray_t = ml::RayAndFrame::ray_with_respect_to_frame(
        &ray,
        &frame);

    CHECK(ml::Vec3(0.0, 4.2, -13.37) == ray_t.support());
    CHECK(ml::VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_ray_in_z_into_frame_rotated_in_z", "[merlict]") {
    ml::Frame frame;
    frame.set_name_pos_rot("frame", ml::VEC3_ORIGIN, ml::Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z);

    ml::Ray ray_t = ml::RayAndFrame::ray_with_respect_to_frame(
        &ray,
        &frame);

    CHECK(ml::VEC3_ORIGIN == ray_t.support());
    CHECK(ml::VEC3_UNIT_Z == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: transform_ray_into_rotated_frame", "[merlict]") {
    ml::Frame frame;
    frame.set_name_pos_rot("frame", ml::VEC3_ORIGIN, ml::Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_X);

    ml::Ray ray_t = ml::RayAndFrame::ray_with_respect_to_frame(
        &ray,
        &frame);

    CHECK(ml::VEC3_ORIGIN == ray_t.support());
    CHECK(ml::VEC3_UNIT_Y == ray_t.direction());
}

TEST_CASE("RayAndFrameTest: causal_intersection", "[merlict]") {
    ml::Frame spheres_in_a_row;
    spheres_in_a_row.set_name_pos_rot("spheres", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Sphere* s0 = spheres_in_a_row.add<ml::Sphere>();
    s0->set_name_pos_rot("s0", ml::Vec3(0.0, 0.0, 0.0), ml::ROT3_UNITY);
    s0->set_radius(1.0);
    ml::Sphere* s1 = spheres_in_a_row.add<ml::Sphere>();
    s1->set_name_pos_rot("s1", ml::Vec3(0.0, 3.0, 0.0), ml::ROT3_UNITY);
    s1->set_radius(1.0);
    ml::Sphere* s2 = spheres_in_a_row.add<ml::Sphere>();
    s2->set_name_pos_rot("s2", ml::Vec3(0.0, 6.0, 0.0), ml::ROT3_UNITY);
    s2->set_radius(1.0);
    ml::Sphere* s3 = spheres_in_a_row.add<ml::Sphere>();
    s3->set_name_pos_rot("s3", ml::Vec3(0.0, 9.0, 0.0), ml::ROT3_UNITY);
    s3->set_radius(1.0);
    ml::Sphere* s4 = spheres_in_a_row.add<ml::Sphere>();
    s4->set_name_pos_rot("s4", ml::Vec3(0.0, 12.0, 0.0), ml::ROT3_UNITY);
    s4->set_radius(1.0);

    spheres_in_a_row.init_tree_based_on_mother_child_relations();
}
