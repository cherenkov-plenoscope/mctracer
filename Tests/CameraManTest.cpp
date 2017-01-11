#include "gtest/gtest.h"
#include "Visual/CameraMan/CameraMan.h"
#include "Visual/PinHoleCamera.h"

class CameraManTest : public ::testing::Test {
protected:
	PinHoleCamera *cam;	
	double initial_FoV_in_rad;

	CameraManTest() {}

	virtual ~CameraManTest() {}

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		Vec3 pos(0.0,0.0,0.0);
		Rot3 rot(0.0,0.0,0.0);
		initial_FoV_in_rad = Deg2Rad(120.0);

		cam = new PinHoleCamera("my_cam",640, 480);
		cam->update_position_and_orientation(pos, rot);
		cam->set_FoV_in_rad(initial_FoV_in_rad);
	}

	virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(CameraManTest, creation) {

	EXPECT_EQ(initial_FoV_in_rad, cam->get_FoV_in_rad());

	CameraMan::FieldOfView FoVCamMan( cam );

	EXPECT_NE(FoVCamMan.get_default_FoV_in_rad(), initial_FoV_in_rad);
	EXPECT_EQ(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, increase_FoV) {

	CameraMan::FieldOfView FoVCamMan( cam );

	for(int i=0; i<250; i++)
		FoVCamMan.increase_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(160.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, decrease_FoV) {

	CameraMan::FieldOfView FoVCamMan( cam );

	for(int i=0; i<250; i++)
		FoVCamMan.decrease_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(0.003));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, default_FoV) {

	CameraMan::FieldOfView FoVCamMan( cam );
	
	FoVCamMan.increase_when_possible();

	EXPECT_NE(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());

	FoVCamMan.set_default_FoV();

	EXPECT_EQ(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, increase_and_decrease_FoV) {

	CameraMan::FieldOfView FoVCamMan( cam );

	FoVCamMan.set_verbosity(false);

	for(int i=0; i<250; i++)
		FoVCamMan.decrease_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(0.003));

	for(int i=0; i<250; i++)
		FoVCamMan.increase_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(160.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, default_rotation) {

	Rot3 non_default_rotation(1.2,3.4,5.6);
	Rot3 looking_in_pos_x_dir(0.0,Deg2Rad(-90.0),0.0);

	cam->update_orientation(non_default_rotation);
	CameraMan::Rotation rot_operator( cam );
	rot_operator.set_default_rotation(looking_in_pos_x_dir);

	EXPECT_EQ(looking_in_pos_x_dir, cam->get_rotation_in_world());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, look_up) {

	CameraMan::Rotation rot_operator( cam );
	rot_operator.set_default_rotation(Rot3(0.0,Deg2Rad(-90.0),0.0));

	for(int i=0; i<50; i++)
		rot_operator.look_further_up_when_possible();

	EXPECT_GT(Deg2Rad( 0.1), cam->get_rotation_in_world().get_rot_y());
	EXPECT_LT(Deg2Rad(-0.1), cam->get_rotation_in_world().get_rot_y());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, look_down) {

	CameraMan::Rotation rot_operator( cam );
	rot_operator.set_default_rotation(Rot3(0.0,Deg2Rad(-90.0),0.0));

	for(int i=0; i<50; i++)
		rot_operator.look_further_down_when_possible();

	EXPECT_GT(Deg2Rad(-179.9), cam->get_rotation_in_world().get_rot_y());
	EXPECT_LT(Deg2Rad(-180.1), cam->get_rotation_in_world().get_rot_y());
}