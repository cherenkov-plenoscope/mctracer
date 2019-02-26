// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace relleums;

struct camera_operator_Test {
    visual::PinHoleCamera cam;
    double initial_FoV_in_rad;

    camera_operator_Test(): cam(visual::PinHoleCamera("my_cam", 640, 480)) {
        initial_FoV_in_rad = deg2rad(120.0);
        cam.update_position_and_orientation(Vec3(0, 0, 0), Rot3(0, 0, 0));
        cam.set_FoV_in_rad(initial_FoV_in_rad);
    }
};

TEST_CASE("camera_operatorTest: creation", "[mctracer]") {
    camera_operator_Test ct;
    CHECK(ct.cam.get_FoV_in_rad() == ct.initial_FoV_in_rad);
    visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    CHECK(ct.initial_FoV_in_rad != FoVCamMan.default_fov);
    CHECK(ct.cam.get_FoV_in_rad() == FoVCamMan.default_fov);
}

TEST_CASE("camera_operatorTest: increase_FoV", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    CHECK(ct.cam.get_FoV_in_rad() > deg2rad(160.0));
    CHECK(ct.cam.get_FoV_in_rad() < deg2rad(180.0));
}

TEST_CASE("camera_operatorTest: decrease_FoV", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    CHECK(ct.cam.get_FoV_in_rad() > deg2rad(0.0));
    CHECK(ct.cam.get_FoV_in_rad() < deg2rad(0.003));
}

TEST_CASE("camera_operatorTest: default_FoV", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    FoVCamMan.increase_when_possible();
    CHECK(ct.cam.get_FoV_in_rad() != FoVCamMan.default_fov);
    FoVCamMan.set_default();
    CHECK(ct.cam.get_FoV_in_rad() == FoVCamMan.default_fov);
}

TEST_CASE("camera_operatorTest: increase_and_decrease_FoV", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::FieldOfView FoVCamMan(&ct.cam);
    FoVCamMan.verbose = false;
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    CHECK(ct.cam.get_FoV_in_rad() > deg2rad(0.0));
    CHECK(ct.cam.get_FoV_in_rad() < deg2rad(0.003));
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    CHECK(ct.cam.get_FoV_in_rad() > deg2rad(160.0));
    CHECK(ct.cam.get_FoV_in_rad() < deg2rad(180.0));
}

TEST_CASE("camera_operatorTest: default_rotation", "[mctracer]") {
    camera_operator_Test ct;
    Rot3 non_default_rotation(1.2, 3.4, 5.6);
    Rot3 looking_in_pos_x_dir(0.0, deg2rad(-90.0), 0.0);
    ct.cam.update_orientation(non_default_rotation);
    visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(looking_in_pos_x_dir);
    CHECK(ct.cam.get_rotation_in_world() == looking_in_pos_x_dir);
}

TEST_CASE("camera_operatorTest: look_up", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(Rot3(0.0, deg2rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_up_when_possible();
    CHECK(ct.cam.get_rotation_in_world().get_rot_y() < deg2rad(0.1));
    CHECK(ct.cam.get_rotation_in_world().get_rot_y() > deg2rad(-0.1));
}

TEST_CASE("camera_operatorTest: look_down", "[mctracer]") {
    camera_operator_Test ct;
    visual::camera_operator::Rotation rot_operator(&ct.cam);
    rot_operator.set_default_rotation(Rot3(0.0, deg2rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_down_when_possible();
    CHECK(ct.cam.get_rotation_in_world().get_rot_y() < deg2rad(-179.9));
    CHECK(ct.cam.get_rotation_in_world().get_rot_y() > deg2rad(-180.1));
}
