#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "CameraMan.h"
#include "CameraManForStereo3D.h"

using namespace std;

class CameraManTest : public ::testing::Test {
protected:
	PinHoleCamera *cam;	
	double initial_FoV_in_rad;

	CameraManTest() {}

	virtual ~CameraManTest() {}

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		Vector3D pos(0.0,0.0,0.0);
		Rotation3D rot(0.0,0.0,0.0);
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

	CameraManFoV FoVCamMan( cam );

	EXPECT_NE(FoVCamMan.get_default_FoV_in_rad(), initial_FoV_in_rad);
	EXPECT_EQ(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, increase_FoV) {

	CameraManFoV FoVCamMan( cam );

	for(int i=0; i<25; i++)
		FoVCamMan.increase_FoV_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(170.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, decrease_FoV) {

	CameraManFoV FoVCamMan( cam );

	for(int i=0; i<25; i++)
		FoVCamMan.decrease_FoV_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(10.0));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, default_FoV) {

	CameraManFoV FoVCamMan( cam );
	
	FoVCamMan.increase_FoV_when_possible();

	EXPECT_NE(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());

	FoVCamMan.set_default_FoV();

	EXPECT_EQ(FoVCamMan.get_default_FoV_in_rad(), cam->get_FoV_in_rad());
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, increase_and_decrease_FoV) {

	CameraManFoV FoVCamMan( cam );

	FoVCamMan.set_verbosity(false);

	for(int i=0; i<25; i++)
		FoVCamMan.decrease_FoV_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(0.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(10.0));

	for(int i=0; i<50; i++)
		FoVCamMan.increase_FoV_when_possible();

	EXPECT_TRUE(cam->get_FoV_in_rad() > Deg2Rad(170.0));
	EXPECT_TRUE(cam->get_FoV_in_rad() < Deg2Rad(180.0));
}
//------------------------------------------------------------------------------
TEST_F(CameraManTest, StereoCameraMan) {

	CameraManForStereo3D stereo_operator( cam );
}