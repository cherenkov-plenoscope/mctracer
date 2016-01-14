#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Geometry/HexagonalPrismZ.h"

using namespace std;

class XyPlaneRayEquationTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, frontal_hit) {

  for(uint i=-100; i<100; i++) {
    
    double sup_z = double(i)*1.337;
    Ray ray(Vector3D(0.0, 0.0, sup_z),Vector3D(0.0, 0.0, 1.0));

    XyPlaneRayIntersectionEquation eq(&ray);

    ASSERT_TRUE(eq.has_solution());
    EXPECT_EQ( sup_z, eq.get_ray_parameter_for_intersection());
    EXPECT_EQ( sup_z<0.0, eq.has_causal_solution());
  }
}
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, no_hit_at_all) {

  Ray ray(Vector3D(0.0, 0.0, 1.0), Vector3D(1.0, 0.0, 0.0));

  XyPlaneRayIntersectionEquation eq(&ray);

  EXPECT_FALSE(eq.has_solution());
}
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, always_ez_normal_vector) {
  
  Ray ray(Vector3D(0.0, 0.0, 1.0), Vector3D(1.0, 0.0, 0.0));
  XyPlaneRayIntersectionEquation eq(&ray);

  EXPECT_EQ(Vector3D::unit_z, eq.get_plane_normal_vector());
}