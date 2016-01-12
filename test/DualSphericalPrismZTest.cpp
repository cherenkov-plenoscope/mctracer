#include "gtest/gtest.h"
#include "DualSphericalPrismZ.h"

class DualSphericalPrismZTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(DualSphericalPrismZTest, is_inside) {
	DualSphericalPrismZ bound;	

	bound.set_x_hight_and_y_width(1.0, 2.0);

	Vector3D v(0.0, 0.0, 0.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(0.0, 0.0, 1.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(0.0, 0.0, -1.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(0.0, +0.99, 0.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(0.0, -0.99, 0.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(+0.49, 0.0, 0.0);
	EXPECT_TRUE(bound.is_inside(&v));

	v.set(-0.49, 0.0, 0.0);
	EXPECT_TRUE(bound.is_inside(&v));
}
//----------------------------------------------------------------------
TEST_F(DualSphericalPrismZTest, is_outside) {
	DualSphericalPrismZ bound;	

	bound.set_x_hight_and_y_width(1.0, 2.0);

	Vector3D v;

	v.set(+0.49, +0.99, 0.0);
	EXPECT_FALSE(bound.is_inside(&v));

	v.set(+0.49, -0.99, 0.0);
	EXPECT_FALSE(bound.is_inside(&v));

	v.set(-0.49, +0.99, 0.0);
	EXPECT_FALSE(bound.is_inside(&v));

	v.set(-0.49, -0.99, 0.0);
	EXPECT_FALSE(bound.is_inside(&v));
}