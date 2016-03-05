#include "gtest/gtest.h"
#include "Core/Rotation3D.h"

using namespace std;

// The fixture for testing class Foo.
class Rotation3DTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(Rotation3DTest, ConstructorAndGetter) {
  // xyz mode
  const double x = -3.141;
  const double y = 2.0;
  const double z = 1.0;
  Rotation3D r(x,y,z); 
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.uses_xyz_angels());

  // axis mode
  Vector3D v(0.0,0.0,1.0);
  double  angle = 1.52;

  Rotation3D p(v,angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.x(), (p.get_rot_axis()).x());
  EXPECT_EQ(v.y(), (p.get_rot_axis()).y());
  EXPECT_EQ(v.z(), (p.get_rot_axis()).z());
  EXPECT_FALSE(p.uses_xyz_angels());
}
//------------------------------------------------------------------------------
TEST_F(Rotation3DTest, SetterAndGetter) {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rotation3D r;   r.set(x,y,z); 
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.uses_xyz_angels());

  // axis mode
  Vector3D v(0.0,0.0,1.0);
  double  angle = 1.52;

  Rotation3D p; p.set(v,angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.x(), (p.get_rot_axis()).x());
  EXPECT_EQ(v.y(), (p.get_rot_axis()).y());
  EXPECT_EQ(v.z(), (p.get_rot_axis()).z());
  EXPECT_FALSE(p.uses_xyz_angels());
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
TEST_F(Rotation3DTest, zoro_rot_angle) {

  Rotation3D r(Vector3D::null, 0.0); 
  EXPECT_EQ(Rotation3D::null ,r); 

  Rotation3D s(Vector3D::unit_z, 0.0); 
  EXPECT_EQ(Rotation3D::null ,s); 

  Rotation3D t(Vector3D::unit_y, 0.0); 
  EXPECT_EQ(Rotation3D::null ,t); 
}