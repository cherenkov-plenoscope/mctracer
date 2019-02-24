// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "gtest/gtest.h"
#include "Core/Rot3.h"

using namespace relleums;

class Rot3Test : public ::testing::Test {};

TEST_F(Rot3Test, ctor_xyz_mode) {
  const double x = -3.141;
  const double y = 2.0;
  const double z = 1.0;
  Rot3 r(x, y, z);
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.uses_xyz_angels());
}

TEST_F(Rot3Test, ctor_axis_mode) {
  Vec3 v(0.0, 0.0, 1.0);
  double angle = 1.52;
  Rot3 p(v, angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.x, (p.get_rot_axis()).x);
  EXPECT_EQ(v.y, (p.get_rot_axis()).y);
  EXPECT_EQ(v.z, (p.get_rot_axis()).z);
  EXPECT_FALSE(p.uses_xyz_angels());
}

TEST_F(Rot3Test, set_xyz_mode) {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rot3 r; r.set(x, y, z);
  EXPECT_EQ(x, r.get_rot_x());
  EXPECT_EQ(y, r.get_rot_y());
  EXPECT_EQ(z, r.get_rot_z());
  EXPECT_TRUE(r.uses_xyz_angels());
}

TEST_F(Rot3Test, set_axis_mode) {
  Vec3 v(0.0, 0.0, 1.0);
  double  angle = 1.52;
  Rot3 p; p.set(v, angle);
  EXPECT_EQ(angle, p.get_rot_angle_in_rad());
  EXPECT_EQ(v.x, (p.get_rot_axis()).x);
  EXPECT_EQ(v.y, (p.get_rot_axis()).y);
  EXPECT_EQ(v.z, (p.get_rot_axis()).z);
  EXPECT_FALSE(p.uses_xyz_angels());
}

TEST_F(Rot3Test, SineAndCosineWhenSetXYZ) {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rot3 r(x, y, z);
  EXPECT_EQ(sin(x), r.sinRx());
  EXPECT_EQ(sin(y), r.sinRy());
  EXPECT_EQ(sin(z), r.sinRz());
  EXPECT_EQ(cos(x), r.cosRx());
  EXPECT_EQ(cos(y), r.cosRy());
  EXPECT_EQ(cos(z), r.cosRz());
}

TEST_F(Rot3Test, zoro_rot_angle) {
  Rot3 r(VEC3_ORIGIN, 0.0);
  EXPECT_EQ(ROT3_UNITY, r);
  Rot3 s(VEC3_UNIT_Z, 0.0);
  EXPECT_EQ(ROT3_UNITY, s);
  Rot3 t(VEC3_UNIT_Y, 0.0);
  EXPECT_EQ(ROT3_UNITY, t);
}