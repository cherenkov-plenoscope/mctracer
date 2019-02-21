// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "gtest/gtest.h"
#include "Core/Frames.h"
#include "Scenery/Primitive/Sphere.h"
using std::string;
using std::stringstream;
using std::vector;
using namespace relleums;

class FramesTest : public ::testing::Test {};

TEST_F(FramesTest, too_close_together_no_frame) {
    vector<Frame*> vf;
    EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_F(FramesTest, too_close_together_one_frame) {
    Frame root;
    root.set_name_pos_rot("root", Vec3::ORIGIN, Rot3::UNITY);
    vector<Frame*> vf; vf.push_back(&root);
    EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_F(FramesTest, too_close_together_true) {
    Frame f1;
    f1.set_name_pos_rot("f1", Vec3::ORIGIN, Rot3::UNITY);

    Frame f2;
    f2.set_name_pos_rot("f2", Vec3::ORIGIN, Rot3::UNITY);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    EXPECT_TRUE(Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_F(FramesTest, too_close_together_false) {
    Frame f1;
    f1.set_name_pos_rot("f1", Vec3::ORIGIN, Rot3::UNITY);

    Frame f2;
    f2.set_name_pos_rot(
        "f2",
        Vec3(0, 0, 10.0*Frame::MIN_STRUCTURE_SIZE),
        Rot3::UNITY);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
    EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}

TEST_F(FramesTest, mean_no_frame) {
    vector<Frame*> vf;
    EXPECT_THROW(
        Frames::mean_of_positions_in_mother(vf),
        std::invalid_argument);
}

TEST_F(FramesTest, optimal_bounding_sphere_pos_no_frame) {
    vector<Frame*> vf;
    EXPECT_THROW(
        Frames::dumb_bounding_sphere_center(vf),
        std::invalid_argument);
}

TEST_F(FramesTest, optimal_bounding_sphere_pos_one_frame) {
    Frame root;
    root.set_name_pos_rot("root", Vec3::UNIT_X, Rot3::UNITY);
    vector<Frame*> vf; vf.push_back(&root);
    EXPECT_EQ(Frames::dumb_bounding_sphere_center(vf), Vec3::UNIT_X);
}

TEST_F(FramesTest, optimal_bounding_sphere_pos_many_frames_symetric) {
    vector<Frame*> vf;
    Frame f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), Rot3::UNITY);
    vf.push_back(&f1);
    Frame f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), Rot3::UNITY);
    vf.push_back(&f2);
    Frame f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), Rot3::UNITY);
    vf.push_back(&f3);
    Frame f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), Rot3::UNITY);
    vf.push_back(&f4);
    Frame f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), Rot3::UNITY);
    vf.push_back(&f5);
    EXPECT_EQ(Vec3(2, 0, 0), Frames::dumb_bounding_sphere_center(vf));
}

TEST_F(FramesTest, optimal_bounding_sphere_pos_many_spheres_symetric) {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), Rot3::UNITY); f1.set_radius(1.0);
    vf.push_back(&f1);
    Sphere f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), Rot3::UNITY); f2.set_radius(1.0);
    vf.push_back(&f2);
    Sphere f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), Rot3::UNITY); f3.set_radius(1.0);
    vf.push_back(&f3);
    Sphere f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), Rot3::UNITY); f4.set_radius(1.0);
    vf.push_back(&f4);
    Sphere f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), Rot3::UNITY); f5.set_radius(1.0);
    vf.push_back(&f5);
    EXPECT_EQ(Vec3(2, 0, 0), Frames::dumb_bounding_sphere_center(vf));
}

TEST_F(FramesTest, optimal_bounding_sphere_pos_many_spheres_asymetric) {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot("f1", Vec3(0, 0, 0), Rot3::UNITY);
    f1.set_radius(1.0); vf.push_back(&f1);
    Sphere f2;
    f2.set_name_pos_rot("f2", Vec3(1, 0, 0), Rot3::UNITY);
    f2.set_radius(1.0); vf.push_back(&f2);
    Sphere f3;
    f3.set_name_pos_rot("f3", Vec3(2, 0, 0), Rot3::UNITY);
    f3.set_radius(1.0); vf.push_back(&f3);
    Sphere f4;
    f4.set_name_pos_rot("f4", Vec3(3, 0, 0), Rot3::UNITY);
    f4.set_radius(1.0); vf.push_back(&f4);
    Sphere f5;
    f5.set_name_pos_rot("f5", Vec3(4, 0, 0), Rot3::UNITY);
    f5.set_radius(5.0); vf.push_back(&f5);
    EXPECT_EQ(Vec3(4, 0, 0), Frames::dumb_bounding_sphere_center(vf));
}

TEST_F(FramesTest, not_optimal_in_symetric_case) {
    vector<Frame*> vf;
    Sphere f1;
    f1.set_name_pos_rot(
        "f1",
        Vec3(cos(Deg2Rad(0.0)), sin(Deg2Rad(0.0)), 0),
        Rot3::UNITY);
    f1.set_radius(0.5); vf.push_back(&f1);

    Sphere f2;
    f2.set_name_pos_rot(
        "f2",
        Vec3(cos(Deg2Rad(120.0)), sin(Deg2Rad(120.0)), 0),
        Rot3::UNITY);
    f2.set_radius(0.5); vf.push_back(&f2);

    Sphere f3;
    f3.set_name_pos_rot(
        "f3",
        Vec3(cos(Deg2Rad(240.0)), sin(Deg2Rad(240.0)), 0),
        Rot3::UNITY);
    f3.set_radius(0.5); vf.push_back(&f3);

    const Vec3 center = Frames::dumb_bounding_sphere_center(vf);
    const double radius = Frames::bounding_sphere_radius(vf, center);

    const double expected_enclosing_radius_for_three_balls = 1.5;
    EXPECT_TRUE(radius > expected_enclosing_radius_for_three_balls);
}
