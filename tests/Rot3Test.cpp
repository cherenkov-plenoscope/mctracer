// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/Rot3.h"

using namespace relleums;



TEST_CASE("Rot3Test: ctor_xyz_mode", "[mctracer]") {
  const double x = -3.141;
  const double y = 2.0;
  const double z = 1.0;
  Rot3 r(x, y, z);
  CHECK(r.get_rot_x() == x);
  CHECK(r.get_rot_y() == y);
  CHECK(r.get_rot_z() == z);
  CHECK(r.uses_xyz_angels());
}

TEST_CASE("Rot3Test: ctor_axis_mode", "[mctracer]") {
  Vec3 v(0.0, 0.0, 1.0);
  double angle = 1.52;
  Rot3 p(v, angle);
  CHECK(p.get_rot_angle_in_rad() == angle);
  CHECK((p.get_rot_axis()).x == v.x);
  CHECK((p.get_rot_axis()).y == v.y);
  CHECK((p.get_rot_axis()).z == v.z);
  CHECK(!p.uses_xyz_angels());
}

TEST_CASE("Rot3Test: set_xyz_mode", "[mctracer]") {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rot3 r; r.set(x, y, z);
  CHECK(r.get_rot_x() == x);
  CHECK(r.get_rot_y() == y);
  CHECK(r.get_rot_z() == z);
  CHECK(r.uses_xyz_angels());
}

TEST_CASE("Rot3Test: set_axis_mode", "[mctracer]") {
  Vec3 v(0.0, 0.0, 1.0);
  double  angle = 1.52;
  Rot3 p; p.set(v, angle);
  CHECK(p.get_rot_angle_in_rad() == angle);
  CHECK((p.get_rot_axis()).x == v.x);
  CHECK((p.get_rot_axis()).y == v.y);
  CHECK((p.get_rot_axis()).z == v.z);
  CHECK(!p.uses_xyz_angels());
}

TEST_CASE("Rot3Test: SineAndCosineWhenSetXYZ", "[mctracer]") {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  Rot3 r(x, y, z);
  CHECK(r.sinRx() == sin(x));
  CHECK(r.sinRy() == sin(y));
  CHECK(r.sinRz() == sin(z));
  CHECK(r.cosRx() == cos(x));
  CHECK(r.cosRy() == cos(y));
  CHECK(r.cosRz() == cos(z));
}

TEST_CASE("Rot3Test: zoro_rot_angle", "[mctracer]") {
  Rot3 r(VEC3_ORIGIN, 0.0);
  CHECK(r == ROT3_UNITY);
  Rot3 s(VEC3_UNIT_Z, 0.0);
  CHECK(s == ROT3_UNITY);
  Rot3 t(VEC3_UNIT_Y, 0.0);
  CHECK(t == ROT3_UNITY);
}
