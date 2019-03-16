// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/merlict.h"
#include "merlict/scenery/primitive/Sphere.h"
namespace ml = merlict;


TEST_CASE("FramesTest: too_close_together_no_frame", "[merlict]") {
    std::vector<ml::Frame*> vf;
    CHECK(!ml::bound::positions_in_mother_too_close(vf));
}

TEST_CASE("FramesTest: too_close_together_one_frame", "[merlict]") {
    ml::Frame root;
    root.set_name_pos_rot("root", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    std::vector<ml::Frame*> vf; vf.push_back(&root);
    CHECK(!ml::bound::positions_in_mother_too_close(vf));
}

TEST_CASE("FramesTest: too_close_together_true", "[merlict]") {
    ml::Frame f1;
    f1.set_name_pos_rot("f1", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Frame f2;
    f2.set_name_pos_rot("f2", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    std::vector<ml::Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    CHECK(ml::bound::positions_in_mother_too_close(vf));
}

TEST_CASE("FramesTest: too_close_together_false", "[merlict]") {
    ml::Frame f1;
    f1.set_name_pos_rot("f1", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Frame f2;
    f2.set_name_pos_rot(
        "f2",
        ml::Vec3(0, 0, 10.0*ml::FRAME_MIN_STRUCTURE_SIZE),
        ml::ROT3_UNITY);

    std::vector<ml::Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    CHECK(!ml::bound::positions_in_mother_too_close(vf));
}

TEST_CASE("FramesTest: mean_no_frame", "[merlict]") {
    std::vector<ml::Frame*> vf;
    CHECK_THROWS_AS(ml::bound::position_mean_in_mother(vf), std::invalid_argument);
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_no_frame", "[merlict]") {
    std::vector<ml::Frame*> vf;
    CHECK_THROWS_AS(ml::bound::bounding_sphere_center_alternative(vf), std::invalid_argument);
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_one_frame", "[merlict]") {
    ml::Frame root;
    root.set_name_pos_rot("root", ml::VEC3_UNIT_X, ml::ROT3_UNITY);
    std::vector<ml::Frame*> vf; vf.push_back(&root);
    CHECK(ml::VEC3_UNIT_X == ml::bound::bounding_sphere_center_alternative(vf));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_frames_symetric", "[merlict]") {
    std::vector<ml::Frame*> vf;
    ml::Frame f1;
    f1.set_name_pos_rot("f1", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
    vf.push_back(&f1);
    ml::Frame f2;
    f2.set_name_pos_rot("f2", ml::Vec3(1, 0, 0), ml::ROT3_UNITY);
    vf.push_back(&f2);
    ml::Frame f3;
    f3.set_name_pos_rot("f3", ml::Vec3(2, 0, 0), ml::ROT3_UNITY);
    vf.push_back(&f3);
    ml::Frame f4;
    f4.set_name_pos_rot("f4", ml::Vec3(3, 0, 0), ml::ROT3_UNITY);
    vf.push_back(&f4);
    ml::Frame f5;
    f5.set_name_pos_rot("f5", ml::Vec3(4, 0, 0), ml::ROT3_UNITY);
    vf.push_back(&f5);
    CHECK(ml::bound::bounding_sphere_center_alternative(vf) == ml::Vec3(2, 0, 0));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_spheres_symetric", "[merlict]") {
    std::vector<ml::Frame*> vf;
    ml::Sphere f1;
    f1.set_name_pos_rot("f1", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
    f1.set_radius(1.0);
    vf.push_back(&f1);
    ml::Sphere f2;
    f2.set_name_pos_rot("f2", ml::Vec3(1, 0, 0), ml::ROT3_UNITY);
    f2.set_radius(1.0);
    vf.push_back(&f2);
    ml::Sphere f3;
    f3.set_name_pos_rot("f3", ml::Vec3(2, 0, 0), ml::ROT3_UNITY);
    f3.set_radius(1.0);
    vf.push_back(&f3);
    ml::Sphere f4;
    f4.set_name_pos_rot("f4", ml::Vec3(3, 0, 0), ml::ROT3_UNITY);
    f4.set_radius(1.0);
    vf.push_back(&f4);
    ml::Sphere f5;
    f5.set_name_pos_rot("f5", ml::Vec3(4, 0, 0), ml::ROT3_UNITY);
    f5.set_radius(1.0);
    vf.push_back(&f5);
    CHECK(ml::bound::bounding_sphere_center_alternative(vf) == ml::Vec3(2, 0, 0));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_spheres_asymetric", "[merlict]") {
    std::vector<ml::Frame*> vf;
    ml::Sphere f1;
    f1.set_name_pos_rot("f1", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
    f1.set_radius(1.0);
    vf.push_back(&f1);
    ml::Sphere f2;
    f2.set_name_pos_rot("f2", ml::Vec3(1, 0, 0), ml::ROT3_UNITY);
    f2.set_radius(1.0);
    vf.push_back(&f2);
    ml::Sphere f3;
    f3.set_name_pos_rot("f3", ml::Vec3(2, 0, 0), ml::ROT3_UNITY);
    f3.set_radius(1.0);
    vf.push_back(&f3);
    ml::Sphere f4;
    f4.set_name_pos_rot("f4", ml::Vec3(3, 0, 0), ml::ROT3_UNITY);
    f4.set_radius(1.0);
    vf.push_back(&f4);
    ml::Sphere f5;
    f5.set_name_pos_rot("f5", ml::Vec3(4, 0, 0), ml::ROT3_UNITY);
    f5.set_radius(5.0);
    vf.push_back(&f5);
    CHECK(ml::bound::bounding_sphere_center_alternative(vf) == ml::Vec3(4, 0, 0));
}

TEST_CASE("FramesTest: not_optimal_in_symetric_case", "[merlict]") {
    std::vector<ml::Frame*> vf;
    ml::Sphere f1;
    f1.set_name_pos_rot(
        "f1",
        ml::Vec3(cos(ml::deg2rad(0.0)), sin(ml::deg2rad(0.0)), 0),
        ml::ROT3_UNITY);
    f1.set_radius(0.5); vf.push_back(&f1);

    ml::Sphere f2;
    f2.set_name_pos_rot(
        "f2",
        ml::Vec3(cos(ml::deg2rad(120.0)), sin(ml::deg2rad(120.0)), 0),
        ml::ROT3_UNITY);
    f2.set_radius(0.5); vf.push_back(&f2);

    ml::Sphere f3;
    f3.set_name_pos_rot(
        "f3",
        ml::Vec3(cos(ml::deg2rad(240.0)), sin(ml::deg2rad(240.0)), 0),
        ml::ROT3_UNITY);
    f3.set_radius(0.5); vf.push_back(&f3);

    const ml::Vec3 center = ml::bound::bounding_sphere_center_alternative(vf);
    const double radius = ml::bound::bounding_sphere_radius(vf, center);

    const double expected_enclosing_radius_for_three_balls = 1.5;
    CHECK(radius > expected_enclosing_radius_for_three_balls);
}
