// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Visual/ApertureCamera.h"

class ApertureCameraTest : public ::testing::Test {};

TEST_F(ApertureCameraTest, set_up) {
  Visual::ApertureCamera Mamiya645("Mamiya645", 640, 480);
  Mamiya645.set_fStop_sesnorWidth_rayPerPixel(2.3, 6e-2, 5);
  Mamiya645.set_FoV_in_rad(Deg2Rad(45.0));
  Vec3 pos(0.0, 0.0, -5.0);
  Rot3 rot(0.0, 0.0, 0.0);
  Mamiya645.update_position_and_orientation(pos, rot);
}
