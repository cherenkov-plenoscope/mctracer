#include "gtest/gtest.h"
#include "Scenery/Geometry/ZaxisCylinderRayIntersectionEquation.h"

class ZaxisCylinderRayIntersectionEquationTest : public ::testing::Test{
};
//------------------------------------------------------------------------------
TEST_F(ZaxisCylinderRayIntersectionEquationTest, start_inside_run_in_z_no_hit) {

  Ray ray(Vec3::null, Vec3::null);

  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  EXPECT_FALSE(eq.has_solution());
  EXPECT_FALSE(eq.has_causal_solution());
}
//------------------------------------------------------------------------------
TEST_F(ZaxisCylinderRayIntersectionEquationTest, start_inside_and_hit) {
  
  Ray ray(Vec3::null, Vec3(1.0, 0.0, 0.0));
  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  EXPECT_TRUE(eq.has_solution());
  EXPECT_TRUE(eq.one_intersec_behind_and_one_in_front_ray_sup());
  EXPECT_FALSE(eq.both_intersects_in_front_ray_sup());
  EXPECT_TRUE(eq.has_causal_solution());
  EXPECT_EQ(1.0, eq.get_ray_parameter_for_intersection());
}
//------------------------------------------------------------------------------
TEST_F(ZaxisCylinderRayIntersectionEquationTest, start_outside_and_hit) {
  
  Ray ray(Vec3(-2.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0));
  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  EXPECT_TRUE(eq.has_solution());
  EXPECT_TRUE(eq.both_intersects_in_front_ray_sup());
  EXPECT_FALSE(eq.one_intersec_behind_and_one_in_front_ray_sup());
  EXPECT_TRUE(eq.has_causal_solution());
  EXPECT_EQ(1.0, eq.get_ray_parameter_for_intersection());
}