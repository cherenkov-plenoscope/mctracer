#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Ray.h"
#include "Geometry/Sphere.h"

using namespace std;

class RayAndFrameTest : public ::testing::Test {
 protected:

	Sphere frame_r_4p2m;
	const double radius_of_test_frame = 4.2;

	RayAndFrameTest() {
		frame_r_4p2m.set_name_pos_rot(
			"frame_with_radius=4.2m", 
			Vector3D::null,
			Rotation3D::null
		);

		frame_r_4p2m.set_sphere_radius(radius_of_test_frame);

		// call this, to make the tests magically pass!
		frame_r_4p2m.get_print();
	}

	virtual ~RayAndFrameTest() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, frame_has_bounding_sphere) {
	EXPECT_EQ(
		radius_of_test_frame, 
		frame_r_4p2m.get_radius_of_sphere_enclosing_all_children()
	);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, frontal_hit_bounding_sphere) {
	double z_below_bounding_sphere = -10.0;

	Ray my_ray(
		Vector3D(0.0, 0.0, z_below_bounding_sphere), Vector3D::unit_z);

	EXPECT_TRUE(my_ray.has_intersection_with_bounding_sphere_of(&frame_r_4p2m));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, no_hit_with_bounding_sphere_behind_support) {
	double z_above_bounding_sphere = 10.0;

	Ray my_ray(Vector3D(0.0, 0.0, z_above_bounding_sphere), Vector3D::unit_z);

	EXPECT_FALSE(my_ray.has_intersection_with_bounding_sphere_of(&frame_r_4p2m));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, inner_hit_bounding_sphere) {
	double z_inside_bounding_sphere = 0.0;

	Ray my_ray(Vector3D(0.0, 0.0, z_inside_bounding_sphere), Vector3D::unit_z);

	EXPECT_TRUE(my_ray.has_intersection_with_bounding_sphere_of(&frame_r_4p2m));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, frontal_and_steep_hits) {
	for(uint i=0; i<1e3; i++) {
		double offset_in_y = i*1e-2;
		double z_below_bounding_sphere = -10.0;

		Ray my_ray(Vector3D(0.0, offset_in_y, z_below_bounding_sphere), Vector3D::unit_z);

		if(fabs(offset_in_y) > radius_of_test_frame ) {
			EXPECT_FALSE(
				my_ray.has_intersection_with_bounding_sphere_of(&frame_r_4p2m)
			);
		}else{
			EXPECT_TRUE(
				my_ray.has_intersection_with_bounding_sphere_of(&frame_r_4p2m)
			);
		}
	}
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, support_inside_bounding_sphere) {
	double z_inside_bounding_sphere = 0.0;

	Ray my_ray(Vector3D(0.0, 0.0, z_inside_bounding_sphere), Vector3D::unit_z);

	EXPECT_TRUE(
		my_ray.support_of_ray_is_inside_bounding_sphere_of(&frame_r_4p2m)
	);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, support_outside_bounding_sphere) {
	double z_outside_bounding_sphere = 10.0;

	Ray my_ray(Vector3D(0.0, 0.0, z_outside_bounding_sphere), Vector3D::unit_z);

	EXPECT_FALSE(
		my_ray.support_of_ray_is_inside_bounding_sphere_of(&frame_r_4p2m)
	);
}
//------------------------------------------------------------------------------