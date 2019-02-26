// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/merlict.h"
#include "merlict/scenery/primitive/Sphere.h"
using std::string;
using std::stringstream;
using std::vector;
using namespace merlict;



TEST_CASE("FramesTest: too_close_together_no_frame", "[mctracer]") {
    vector<Frame*> vf;
    CHECK(!Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_CASE("FramesTest: too_close_together_one_frame", "[mctracer]") {
    Frame root;
    root.set_name_pos_rot("root", VEC3_ORIGIN, ROT3_UNITY);
    vector<Frame*> vf; vf.push_back(&root);
    CHECK(!Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_CASE("FramesTest: too_close_together_true", "[mctracer]") {
    Frame f1;
    f1.set_name_pos_rot("f1", VEC3_ORIGIN, ROT3_UNITY);

    Frame f2;
    f2.set_name_pos_rot("f2", VEC3_ORIGIN, ROT3_UNITY);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    CHECK(Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_CASE("FramesTest: too_close_together_false", "[mctracer]") {
    Frame f1;
    f1.set_name_pos_rot("f1", VEC3_ORIGIN, ROT3_UNITY);

    Frame f2;
    f2.set_name_pos_rot(
        "f2",
        Vec3(0, 0, 10.0*FRAME_MIN_STRUCTURE_SIZE),
        ROT3_UNITY);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    CHECK(!Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_CASE("FramesTest: mean_no_frame", "[mctracer]") {
    vector<Frame*> vf;
    CHECK_THROWS_AS(Frames::mean_of_positions_in_mother(vf), std::invalid_argument);
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_no_frame", "[mctracer]") {
    vector<Frame*> vf;
    CHECK_THROWS_AS(Frames::dumb_bounding_sphere_center(vf), std::invalid_argument);
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_one_frame", "[mctracer]") {
    Frame root;
    root.set_name_pos_rot("root", VEC3_UNIT_X, ROT3_UNITY);
    vector<Frame*> vf; vf.push_back(&root);
    CHECK(VEC3_UNIT_X == Frames::dumb_bounding_sphere_center(vf));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_frames_symetric", "[mctracer]") {
    vector<Frame*> vf;
    Frame f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), ROT3_UNITY);
    vf.push_back(&f1);
    Frame f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), ROT3_UNITY);
    vf.push_back(&f2);
    Frame f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), ROT3_UNITY);
    vf.push_back(&f3);
    Frame f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), ROT3_UNITY);
    vf.push_back(&f4);
    Frame f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), ROT3_UNITY);
    vf.push_back(&f5);
    CHECK(Frames::dumb_bounding_sphere_center(vf) == Vec3(2, 0, 0));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_spheres_symetric", "[mctracer]") {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), ROT3_UNITY); f1.set_radius(1.0);
    vf.push_back(&f1);
    Sphere f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), ROT3_UNITY); f2.set_radius(1.0);
    vf.push_back(&f2);
    Sphere f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), ROT3_UNITY); f3.set_radius(1.0);
    vf.push_back(&f3);
    Sphere f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), ROT3_UNITY); f4.set_radius(1.0);
    vf.push_back(&f4);
    Sphere f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), ROT3_UNITY); f5.set_radius(1.0);
    vf.push_back(&f5);
    CHECK(Frames::dumb_bounding_sphere_center(vf) == Vec3(2, 0, 0));
}

TEST_CASE("FramesTest: optimal_bounding_sphere_pos_many_spheres_asymetric", "[mctracer]") {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), ROT3_UNITY);
    f1.set_radius(1.0); vf.push_back(&f1);
    Sphere f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), ROT3_UNITY);
    f2.set_radius(1.0); vf.push_back(&f2);
    Sphere f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), ROT3_UNITY);
    f3.set_radius(1.0); vf.push_back(&f3);
    Sphere f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), ROT3_UNITY);
    f4.set_radius(1.0); vf.push_back(&f4);
    Sphere f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), ROT3_UNITY);
    f5.set_radius(5.0); vf.push_back(&f5);
    CHECK(Frames::dumb_bounding_sphere_center(vf) == Vec3(4, 0, 0));
}

TEST_CASE("FramesTest: not_optimal_in_symetric_case", "[mctracer]") {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot(
        "f1",
        Vec3(cos(deg2rad(0.0)), sin(deg2rad(0.0)), 0),
        ROT3_UNITY);
    f1.set_radius(0.5); vf.push_back(&f1);

    Sphere f2;
    f2.set_name_pos_rot(
        "f2",
        Vec3(cos(deg2rad(120.0)), sin(deg2rad(120.0)), 0),
        ROT3_UNITY);
    f2.set_radius(0.5); vf.push_back(&f2);

    Sphere f3;
    f3.set_name_pos_rot(
        "f3",
        Vec3(cos(deg2rad(240.0)), sin(deg2rad(240.0)), 0),
        ROT3_UNITY);
    f3.set_radius(0.5); vf.push_back(&f3);

    const Vec3 center = Frames::dumb_bounding_sphere_center(vf);
    const double radius = Frames::bounding_sphere_radius(vf, center);

    const double expected_enclosing_radius_for_three_balls = 1.5;
    CHECK(radius > expected_enclosing_radius_for_three_balls);
}
