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
  EXPECT_EQ(1, prop_ray.get_number_of_interactions_so_far()); //creation itselt
  EXPECT_EQ(1337, prop_ray.get_id());
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, Constructor_using_ray_to_be_expanded) {

  RayForPropagation prop_ray(support, direction);
  prop_ray.set_id(1337);

  RayForPropagation reflected_prop_ray(&prop_ray);

  EXPECT_EQ(1, reflected_prop_ray.get_number_of_interactions_so_far() );
  EXPECT_EQ(1337, reflected_prop_ray.get_id());

  reflected_prop_ray.SetSupport(support);
  reflected_prop_ray.SetDirection(direction);

  EXPECT_EQ(support, reflected_prop_ray.Support() );
  EXPECT_EQ(direction, reflected_prop_ray.Direction() );
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, direction_normalized) {

  RayForPropagation prop_ray(support, direction*42.0);
  EXPECT_EQ(1.0, prop_ray.Direction().norm());
}
//------------------------------------------------------------------------------
TEST_F(RayForPropagationTest, distance_passed_after_creation) {

  RayForPropagation prop_ray(support, direction);
  EXPECT_EQ(0.0, prop_ray.get_accumulative_distance());
}