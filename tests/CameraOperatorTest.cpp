// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "viewer/camera_operator/camera_operator.h"
namespace ml = merlict;


struct camera_operator_Test {
    ml::visual::PinHoleCamera cam;
    double initial_FoV_in_rad;

    camera_operator_Test(): cam(ml::visual::PinHoleCamera("my_cam", 640, 480)) {
        initial_FoV_in_rad = ml::deg2rad(120.0);
        cam.set_pos_rot_fov(
            ml::Vec3(0, 0, 0),
            ml::Rot3(0, 0, 0),
            initial_FoV_in_rad);
    }
};

TEST_CASE("camera_operatorTest: creation", "[merlict]") {
    camera_operator_Test ct;
    CHECK(ct.cam.field_of_view() == ct.initial_FoV_in_rad);
    ml::visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    CHECK(ct.initial_FoV_in_rad != FoVCamMan.default_fov);
    CHECK(ct.cam.field_of_view() == FoVCamMan.default_fov);
}

TEST_CASE("camera_operatorTest: increase_FoV", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    CHECK(ct.cam.field_of_view() > ml::deg2rad(160.0));
    CHECK(ct.cam.field_of_view() < ml::deg2rad(180.0));
}

TEST_CASE("camera_operatorTest: decrease_FoV", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    CHECK(ct.cam.field_of_view() > ml::deg2rad(0.0));
    CHECK(ct.cam.field_of_view() < ml::deg2rad(0.003));
}

TEST_CASE("camera_operatorTest: default_FoV", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    FoVCamMan.increase_when_possible();
    CHECK(ct.cam.field_of_view() != FoVCamMan.default_fov);
    FoVCamMan.set_default();
    CHECK(ct.cam.field_of_view() == FoVCamMan.default_fov);
}

TEST_CASE("camera_operatorTest: increase_and_decrease_FoV", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    FoVCamMan.verbose = false;
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    CHECK(ct.cam.field_of_view() > ml::deg2rad(0.0));
    CHECK(ct.cam.field_of_view() < ml::deg2rad(0.003));
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    CHECK(ct.cam.field_of_view() > ml::deg2rad(160.0));
    CHECK(ct.cam.field_of_view() < ml::deg2rad(180.0));
}

TEST_CASE("camera_operatorTest: default_rotation", "[merlict]") {
    camera_operator_Test ct;
    ml::Rot3 non_default_rotation(1.2, 3.4, 5.6);
    ml::Rot3 looking_in_pos_x_dir(0.0, ml::deg2rad(-90.0), 0.0);
    ct.cam.set_pos_rot_fov(
        ct.cam.position(),
        non_default_rotation,
        ct.cam.field_of_view());
    ml::visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(looking_in_pos_x_dir);
    CHECK(ct.cam.rotation() == looking_in_pos_x_dir);
}

TEST_CASE("camera_operatorTest: look_up", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(ml::Rot3(0.0, ml::deg2rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_up_when_possible();
    CHECK(ct.cam.rotation().rot_y() < ml::deg2rad(0.1));
    CHECK(ct.cam.rotation().rot_y() > ml::deg2rad(-0.1));
}

TEST_CASE("camera_operatorTest: look_down", "[merlict]") {
    camera_operator_Test ct;
    ml::visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(ml::Rot3(0.0, ml::deg2rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_down_when_possible();
    CHECK(ct.cam.rotation().rot_y() < ml::deg2rad(-179.9));
    CHECK(ct.cam.rotation().rot_y() > ml::deg2rad(-180.1));
}
