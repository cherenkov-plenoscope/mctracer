// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("ApertureCameraTest: set_up", "[merlict]") {
  ml::visual::ApertureCamera Mamiya645("Mamiya645", 640, 480);
  Mamiya645.set_fStop_sesnorWidth(2.3, 6e-2);
  Mamiya645.set_pos_rot_fov(
  	ml::Vec3(0, 0, -5),
  	ml::Rot3(0, 0, 0),
  	ml::deg2rad(45));
}
