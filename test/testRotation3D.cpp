#include <iostream> 
#include <math.h>

#include "gtest/gtest.h"
#include "../Rotation3D.h"
#include "../Vector3D.h"

using namespace std;

// The fixture for testing class Foo.
class Rotation3DTest : public ::testing::Test {
  protected:
  // You can remove any or all of the following functions if its body
  // is empty.


	
  Rotation3DTest() {
    // You can do set-up work for each test here.
  }

  virtual ~Rotation3DTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).

  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(Rotation3DTest, ConstructorAndGetter) {
  // xyz mode
  const double x = -3.141;
  const double y = 2.0;
  const double z = 1.0;
  Rotation3D r(x,y,z); 
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.get_flag_is_rot_angles_xyz());

  // axis mode
  Vector3D v(0.0,0.0,1.0);
  double  angle = 1.52;

  Rotation3D p(v,angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.get_x(), (p.get_rot_axis()).get_x());
  EXPECT_EQ(v.get_y(), (p.get_rot_axis()).get_y());
  EXPECT_EQ(v.get_z(), (p.get_rot_axis()).get_z());
  EXPECT_FALSE(p.get_flag_is_rot_angles_xyz());
}
//----------------------------------------------------------------------
TEST_F(Rotation3DTest, SetterAndGetter) {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rotation3D r;   r.set(x,y,z); 
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.get_flag_is_rot_angles_xyz());

  // axis mode
  Vector3D v(0.0,0.0,1.0);
  double  angle = 1.52;

  Rotation3D p; p.set(v,angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.get_x(), (p.get_rot_axis()).get_x());
  EXPECT_EQ(v.get_y(), (p.get_rot_axis()).get_y());
  EXPECT_EQ(v.get_z(), (p.get_rot_axis()).get_z());
  EXPECT_FALSE(p.get_flag_is_rot_angles_xyz());
}
//----------------------------------------------------------------------
TEST_F(Rotation3DTest, SineAndCosineWhenSetXYZ) {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rotation3D r(x,y,z); 

  EXPECT_EQ(sin(x), r.sinRx()); 
  EXPECT_EQ(sin(y), r.sinRy()); 
  EXPECT_EQ(sin(z), r.sinRz()); 
  EXPECT_EQ(cos(x), r.cosRx()); 
  EXPECT_EQ(cos(y), r.cosRy()); 
  EXPECT_EQ(cos(z), r.cosRz()); 
}
//----------------------------------------------------------------------
/*
TEST_F(Rotation3DTest, SineAndCosineWhenSetVecAnglge) {
  // axis mode
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Vector3D v(0.0,0.0,1.0);
  double  angle = 1.52;
  Rotation3D p(v,angle); 

  EXPECT_EQ(sin(x), p.sinRx()); 
  EXPECT_EQ(sin(y), p.sinRy()); 
  EXPECT_EQ(sin(z), p.sinRz()); 
  EXPECT_EQ(cos(x), p.cosRx()); 
  EXPECT_EQ(cos(y), p.cosRy()); 
  EXPECT_EQ(cos(z), p.cosRz()); 
}
*/