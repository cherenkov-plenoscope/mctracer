// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("ApertureCameraTest: set_up", "[merlict]") {
  ml::visual::ApertureCamera Mamiya645("Mamiya645", 640, 480);
  Mamiya645.set_fStop_sesnorWidth(2.3, 6e-2);
  Mamiya645.set_FoV_in_rad(ml::deg2rad(45.0));
  ml::Vec3 pos(0.0, 0.0, -5.0);
  ml::Rot3 rot(0.0, 0.0, 0.0);
  Mamiya645.update_position_and_orientation(pos, rot);
}
