#include "gtest/gtest.h"
#include "Core/Frames.h"
#include "Scenery/Primitive/Sphere.h"
class FramesTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FramesTest, too_close_together_no_frame) {

	vector<Frame*> vf;
	EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, too_close_together_one_frame) {

    Frame root;
    root.set_name_pos_rot("root", Vec3::null, Rot3::null);
    vector<Frame*> vf; vf.push_back(&root);
	EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, too_close_together_true) {

    Frame f1;
    f1.set_name_pos_rot("f1", Vec3::null, Rot3::null);

    Frame f2;
    f2.set_name_pos_rot("f2", Vec3::null, Rot3::null);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
	EXPECT_TRUE(Frames::positions_in_mother_are_too_close_together(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, too_close_together_false) {

    Frame f1;
    f1.set_name_pos_rot("f1", Vec3::null, Rot3::null);

    Frame f2;
    f2.set_name_pos_rot("f2", Vec3(0,0,10.0*Frame::minimal_structure_size), Rot3::null);

    vector<Frame*> vf; vf.push_back(&f1); vf.push_back(&f2);
	EXPECT_FALSE(Frames::positions_in_mother_are_too_close_together(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, mean_no_frame) {

	vector<Frame*> vf;
	EXPECT_THROW(Frames::get_mean_pos_in_mother(vf), Frames::TooFewFrames);
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, optimal_bounding_sphere_pos_no_frame) {

	vector<Frame*> vf;
	EXPECT_THROW(Frames::optimal_bounding_sphere_center(vf), Frames::TooFewFrames);
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, optimal_bounding_sphere_pos_one_frame) {

    Frame root;
    root.set_name_pos_rot("root", Vec3::unit_x, Rot3::null);
    vector<Frame*> vf; vf.push_back(&root);
	EXPECT_EQ(Frames::optimal_bounding_sphere_center(vf), Vec3::unit_x);
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, optimal_bounding_sphere_pos_many_frames_symetric) {

	vector<Frame*> vf;
    Frame f1; f1.set_name_pos_rot("f1", Vec3(0,0,0), Rot3::null); vf.push_back(&f1);
    Frame f2; f2.set_name_pos_rot("f2", Vec3(1,0,0), Rot3::null); vf.push_back(&f2);
    Frame f3; f3.set_name_pos_rot("f3", Vec3(2,0,0), Rot3::null); vf.push_back(&f3);
   	Frame f4; f4.set_name_pos_rot("f4", Vec3(3,0,0), Rot3::null); vf.push_back(&f4);
    Frame f5; f5.set_name_pos_rot("f5", Vec3(4,0,0), Rot3::null); vf.push_back(&f5);
	EXPECT_EQ(Vec3(2,0,0), Frames::optimal_bounding_sphere_center(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, optimal_bounding_sphere_pos_many_spheres_symetric) {

	vector<Frame*> vf;
    Sphere f1; f1.set_name_pos_rot("f1", Vec3(0,0,0), Rot3::null); f1.set_radius(1.0); vf.push_back(&f1);
    Sphere f2; f2.set_name_pos_rot("f2", Vec3(1,0,0), Rot3::null); f2.set_radius(1.0); vf.push_back(&f2);
    Sphere f3; f3.set_name_pos_rot("f3", Vec3(2,0,0), Rot3::null); f3.set_radius(1.0); vf.push_back(&f3);
   	Sphere f4; f4.set_name_pos_rot("f4", Vec3(3,0,0), Rot3::null); f4.set_radius(1.0); vf.push_back(&f4);
    Sphere f5; f5.set_name_pos_rot("f5", Vec3(4,0,0), Rot3::null); f5.set_radius(1.0); vf.push_back(&f5);
	EXPECT_EQ(Vec3(2,0,0), Frames::optimal_bounding_sphere_center(vf));
}
//------------------------------------------------------------------------------
TEST_F(FramesTest, optimal_bounding_sphere_pos_many_spheres_asymetric) {

	vector<Frame*> vf;
    Sphere f1; f1.set_name_pos_rot("f1", Vec3(0,0,0), Rot3::null); f1.set_radius(1.0); vf.push_back(&f1);
    Sphere f2; f2.set_name_pos_rot("f2", Vec3(1,0,0), Rot3::null); f2.set_radius(1.0); vf.push_back(&f2);
    Sphere f3; f3.set_name_pos_rot("f3", Vec3(2,0,0), Rot3::null); f3.set_radius(1.0); vf.push_back(&f3);
   	Sphere f4; f4.set_name_pos_rot("f4", Vec3(3,0,0), Rot3::null); f4.set_radius(1.0); vf.push_back(&f4);
    Sphere f5; f5.set_name_pos_rot("f5", Vec3(4,0,0), Rot3::null); f5.set_radius(5.0); vf.push_back(&f5);
	EXPECT_EQ(Vec3(4,0,0), Frames::optimal_bounding_sphere_center(vf));
}