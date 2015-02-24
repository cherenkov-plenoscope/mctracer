#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "RayForPropagation.h"

using namespace std;

class RayForPropagationTest : public ::testing::Test {
 protected:

  Vector3D support;
  Vector3D direction;

  RayForPropagationTest() {
    support.set(0.0,0.0,1.0);
    direction.set(0.0,0.0,1.0);
  }

  virtual ~RayForPropagationTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, Constructor_using_vectors) {

  RayForPropagation prop_ray(support, direction);
  prop_ray.set_id(1337);

  EXPECT_EQ(support, prop_ray.Support() );
  EXPECT_EQ(direction, prop_ray.Direction() );
  ASSERT_FALSE(nullptr == prop_ray.get_propagation_history() );
  EXPECT_EQ(0, prop_ray.get_propagation_history()->size() );
  EXPECT_EQ(1337, prop_ray.get_id());
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, Constructor_using_ray_to_be_expanded) {

  RayForPropagation prop_ray(support, direction);
  prop_ray.set_id(1337);

  RayForPropagation reflected_prop_ray(&prop_ray);

  ASSERT_FALSE(nullptr == reflected_prop_ray.get_propagation_history() );
  EXPECT_EQ(0, reflected_prop_ray.get_propagation_history()->size() );
  EXPECT_EQ(1337, reflected_prop_ray.get_id());

  reflected_prop_ray.SetSupport(support);
  reflected_prop_ray.SetDirection(direction);

  EXPECT_EQ(support, reflected_prop_ray.Support() );
  EXPECT_EQ(direction, reflected_prop_ray.Direction() );
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, direction_normalized) {

  RayForPropagation prop_ray(support, direction*42.0);
  EXPECT_EQ(1.0, prop_ray.Direction().norm2());
}