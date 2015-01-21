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

  bool error_detected = false;
  try{
    Sphere N;
    N.set_sphere(1.0);
  }catch(...){
    error_detected = true;
  }
  EXPECT_FALSE(error_detected);
}
//------------------------------------------------------------------------------
TEST_F(SphereTest, set_negativ_radius) {

  bool error_detected = false;
  try{
    Sphere N;
    N.set_sphere(-1.0);
  }catch(std::exception &err){
    error_detected = true;
  }
  EXPECT_TRUE(error_detected) << "Negativ radius of a sphere is not physicaly";
}
//------------------------------------------------------------------------------
TEST_F(SphereTest, set_zero_radius) {
  bool error_detected = false;
  try{
    Sphere N;
    N.set_sphere(0.0);
  }catch(std::exception &err){
    error_detected = true;
  }
  EXPECT_TRUE(error_detected) << "Spheres with zero radius are not allowed! "
  "Imagine setting up such a sphere and wondering why there are never "
  "intersections althoug no other errors occured.";
}