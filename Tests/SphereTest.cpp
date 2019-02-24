// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/primitive/Sphere.h"

using namespace relleums;

class SphereTest : public ::testing::Test {};

TEST_F(SphereTest, set_positiv_radius) {
  Sphere N;
  EXPECT_NO_THROW(N.set_radius(1.0));
}

TEST_F(SphereTest, set_negativ_radius) {
  Sphere N;
  EXPECT_THROW(N.set_radius(-1.0), Sphere::BadRadius) <<
  "Spheres must not have negative radius";
}

TEST_F(SphereTest, set_zero_radius) {
  Sphere N;
  EXPECT_THROW(N.set_radius(0.0), Sphere::BadRadius) <<
  "Spheres with zero radius are not allowed! "
  "Imagine setting up such a sphere and wondering why there are never "
  "intersections althoug no other errors occured.";
}
