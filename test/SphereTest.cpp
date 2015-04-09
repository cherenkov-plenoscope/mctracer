#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Vector3D.h"

using namespace std;

class SphereTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(SphereTest, set_positiv_radius) {
  
  Sphere N;
  EXPECT_NO_THROW( N.set_sphere_radius(1.0) );
}
//------------------------------------------------------------------------------
TEST_F(SphereTest, set_negativ_radius) {

  Sphere N;
  EXPECT_THROW( N.set_sphere_radius(-1.0), TracerException ) << 
  "Spheres must not have negative radius";
}
//------------------------------------------------------------------------------
TEST_F(SphereTest, set_zero_radius) {

  Sphere N;
  EXPECT_THROW( N.set_sphere_radius(0.0), TracerException ) << 
  "Spheres with zero radius are not allowed! "
  "Imagine setting up such a sphere and wondering why there are never "
  "intersections althoug no other errors occured.";
}