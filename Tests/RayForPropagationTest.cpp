#include "gtest/gtest.h"
#include "Core/RayForPropagation.h"

using namespace std;

class RayForPropagationTest : public ::testing::Test {
 protected:

  Vec3 support;
  Vec3 direction;

  RayForPropagationTest() {
    support.set(0.0,0.0,1.0);
    direction.set(0.0,0.0,1.0);
  }
};
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, Constructor_using_vectors) {

  RayForPropagation prop_ray(support, direction);
  prop_ray.set_id(1337);

  EXPECT_EQ(support, prop_ray.get_support() );
  EXPECT_EQ(direction, prop_ray.get_direction() );
  EXPECT_EQ(1, prop_ray.get_number_of_interactions_so_far()); //creation itselt
  EXPECT_EQ(1337, prop_ray.get_id());
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, direction_normalized) {

  RayForPropagation prop_ray(support, direction*42.0);
  EXPECT_EQ(1.0, prop_ray.get_direction().norm());
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, distance_passed_after_creation) {

  RayForPropagation prop_ray(support, direction);
  EXPECT_EQ(0.0, prop_ray.get_accumulative_distance());
}