// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Visual/CameraOperator/CameraOperator.h"
#include "Visual/PinHoleCamera.h"

class CameraOperatorTest : public ::testing::Test {
 protected:
    Visual::PinHoleCamera *cam;
    double initial_FoV_in_rad;

    CameraOperatorTest() {}

    virtual ~CameraOperatorTest() {}

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        Vec3 pos(0.0, 0.0, 0.0);
        Rot3 rot(0.0, 0.0, 0.0);
        initial_FoV_in_rad = Deg2Rad(120.0);

        cam = new Visual::PinHoleCamera("my_cam", 640, 480);
        cam->update_position_and_orientation(pos, rot);
        cam->set_FoV_in_rad(initial_FoV_in_rad);
    }

    virtual void TearDown() {}
};

TEST_F(CameraOperatorTest, creation) {
    EXPECT_EQ(initial_FoV_in_rad, cam->get_FoV_in_rad());
    Visual::CameraOperator::FieldOfView FoVCamMan(cam);
    EXPECT_NE(FoVCamMan.default_fov, initial_FoV_in_rad);
    EXPECT_EQ(FoVCamMan.default_fov, cam->get_FoV_in_rad());
}

TEST_F(CameraOperatorTest, increase_FoV) {
    Visual::CameraOperator::FieldOfView FoVCamMan(cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(160.0));
    EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}

TEST_F(CameraOperatorTest, decrease_FoV) {
    Visual::CameraOperator::FieldOfView FoVCamMan(cam);
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
    EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(0.003));
}

TEST_F(CameraOperatorTest, default_FoV) {
    Visual::CameraOperator::FieldOfView FoVCamMan(cam);
    FoVCamMan.increase_when_possible();
    EXPECT_NE(FoVCamMan.default_fov, cam->get_FoV_in_rad());
    FoVCamMan.set_default();
    EXPECT_EQ(FoVCamMan.default_fov, cam->get_FoV_in_rad());
}

TEST_F(CameraOperatorTest, increase_and_decrease_FoV) {
    Visual::CameraOperator::FieldOfView FoVCamMan(cam);
    FoVCamMan.set_verbosity(false);
    for (int i = 0; i < 250; i++)
        FoVCamMan.decrease_when_possible();
    EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
    EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(0.003));
    for (int i = 0; i < 250; i++)
        FoVCamMan.increase_when_possible();
    EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(160.0));
    EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}

TEST_F(CameraOperatorTest, default_rotation) {
    Rot3 non_default_rotation(1.2, 3.4, 5.6);
    Rot3 looking_in_pos_x_dir(0.0, Deg2Rad(-90.0), 0.0);
    cam->update_orientation(non_default_rotation);
    Visual::CameraOperator::Rotation rot_operator(cam);
    rot_operator.set_default_rotation(looking_in_pos_x_dir);
    EXPECT_EQ(looking_in_pos_x_dir, cam->get_rotation_in_world());
}

TEST_F(CameraOperatorTest, look_up) {
    Visual::CameraOperator::Rotation rot_operator(cam);
    rot_operator.set_default_rotation(Rot3(0.0, Deg2Rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_up_when_possible();
    EXPECT_GT(Deg2Rad(0.1), cam->get_rotation_in_world().get_rot_y());
    EXPECT_LT(Deg2Rad(-0.1), cam->get_rotation_in_world().get_rot_y());
}

TEST_F(CameraOperatorTest, look_down) {
    Visual::CameraOperator::Rotation rot_operator(cam);
    rot_operator.set_default_rotation(Rot3(0.0, Deg2Rad(-90.0), 0.0));
    for (int i = 0; i < 50; i++)
        rot_operator.look_further_down_when_possible();
    EXPECT_GT(Deg2Rad(-179.9), cam->get_rotation_in_world().get_rot_y());
    EXPECT_LT(Deg2Rad(-180.1), cam->get_rotation_in_world().get_rot_y());
}
