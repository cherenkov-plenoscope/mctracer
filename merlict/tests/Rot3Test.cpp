// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/Rot3.h"
namespace ml = merlict;


TEST_CASE("Rot3Test: ctor_xyz_mode", "[merlict]") {
  const double x = -3.141;
  const double y = 2.0;
  const double z = 1.0;
  ml::Rot3 r(x, y, z);
  CHECK(r.rot_x() == x);
  CHECK(r.rot_y() == y);
  CHECK(r.rot_z() == z);
  CHECK(r.uses_xyz_angels());
}

TEST_CASE("Rot3Test: ctor_axis_mode", "[merlict]") {
  ml::Vec3 v(0.0, 0.0, 1.0);
  double angle = 1.52;
  ml::Rot3 p(v, angle);
  CHECK(p.rot_angle() == angle);
  CHECK((p.rot_axis()).x == v.x);
  CHECK((p.rot_axis()).y == v.y);
  CHECK((p.rot_axis()).z == v.z);
  CHECK(!p.uses_xyz_angels());
}

TEST_CASE("Rot3Test: set_xyz_mode", "[merlict]") {
  const double x = -3.141;
  const double y = -2.0;
  const double z = 1.0000;
  ml::Rot3 r; r.set(x, y, z);
  CHECK(r.rot_x() == x);
  CHECK(r.rot_y() == y);
  CHECK(r.rot_z() == z);
  CHECK(r.uses_xyz_angels());
}

TEST_CASE("Rot3Test: set_axis_mode", "[merlict]") {
  ml::Vec3 v(0.0, 0.0, 1.0);
  double  angle = 1.52;
  ml::Rot3 p; p.set(v, angle);
  CHECK(p.rot_angle() == angle);
  CHECK((p.rot_axis()).x == v.x);
  CHECK((p.rot_axis()).y == v.y);
  CHECK((p.rot_axis()).z == v.z);
  CHECK(!p.uses_xyz_angels());
}

TEST_CASE("Rot3Test: zoro_rot_angle", "[merlict]") {
  ml::Rot3 r(ml::VEC3_ORIGIN, 0.0);
  CHECK(r == ml::ROT3_UNITY);
  ml::Rot3 s(ml::VEC3_UNIT_Z, 0.0);
  CHECK(s == ml::ROT3_UNITY);
  ml::Rot3 t(ml::VEC3_UNIT_Y, 0.0);
  CHECK(t == ml::ROT3_UNITY);
}
