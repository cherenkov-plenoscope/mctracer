#include "gtest/gtest.h"
#include "Scenery/Geometry/HexagonalPrismZ.h"
#include "Scenery/Geometry/XyPlaneRayIntersectionEquation.h"
#include "Core/Ray.h"

class XyPlaneRayEquationTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, frontal_hit) {

  for(unsigned int i=-100; i<100; i++) {
    
    double sup_z = double(i)*1.337;
    Ray ray(Vec3(0.0, 0.0, sup_z),Vec3(0.0, 0.0, 1.0));

    XyPlaneRayIntersectionEquation eq(&ray);

    ASSERT_TRUE(eq.has_solution());
    EXPECT_EQ( sup_z, eq.get_ray_parameter_for_intersection());
    EXPECT_EQ( sup_z<0.0, eq.has_causal_solution());
  }
}
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, no_hit_at_all) {

  Ray ray(Vec3(0.0, 0.0, 1.0), Vec3(1.0, 0.0, 0.0));

  XyPlaneRayIntersectionEquation eq(&ray);

  EXPECT_FALSE(eq.has_solution());
}
//------------------------------------------------------------------------------
TEST_F(XyPlaneRayEquationTest, always_ez_normal_vector) {
  
  Ray ray(Vec3(0.0, 0.0, 1.0), Vec3(1.0, 0.0, 0.0));
  XyPlaneRayIntersectionEquation eq(&ray);

  EXPECT_EQ(Vec3::UNIT_Z, eq.get_plane_normal_vector());
}