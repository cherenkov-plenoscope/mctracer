#include "gtest/gtest.h"
#include "Core/Ray.h"
#include "Geometry/Sphere.h"

using namespace std;

class RayTest : public ::testing::Test {
 protected:

  Vec3 support;
  Vec3 direction;

  RayTest() {
    support.set(0.0,0.0,1.0);
    direction.set(0.0,0.0,1.0);
  }

  virtual ~RayTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};
//------------------------------------------------------------------------------
TEST_F(RayTest, Constructor_using_vectors) {

  Ray my_ray(support, direction);

  EXPECT_EQ(support, my_ray.get_support());
  EXPECT_EQ(direction/direction.norm(), my_ray.get_direction()); 
  EXPECT_EQ(1.0, my_ray.get_direction().norm());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, position_on_ray) {
  Ray my_ray(support, direction);

  double ray_parameter = 5.0;
  Vec3 pos_on_ray = support + direction*ray_parameter;
    
  EXPECT_EQ(pos_on_ray, my_ray.get_pos_at(ray_parameter));
}
//------------------------------------------------------------------------------
TEST_F(RayTest, closest_distance_to_point) {
  Ray my_ray(Vec3::null, Vec3(0.0, 0.0, 1.0));

  EXPECT_EQ(0.0, my_ray.get_closest_distance_to_point(Vec3::null));
  EXPECT_EQ(1.0, my_ray.get_closest_distance_to_point(Vec3(1.0, 0.0, 0.0)));
  EXPECT_EQ(1.0, my_ray.get_closest_distance_to_point(Vec3(0.0, 1.0, 0.0)));
  EXPECT_EQ(0.0, my_ray.get_closest_distance_to_point(Vec3(0.0, 0.0, 1.0)));
}
//------------------------------------------------------------------------------
TEST_F(RayTest, set_direction) {
  Ray my_ray(support, direction*42.0);
  my_ray.set_direction(Vec3(1.2, 3.4, 5.6));
  EXPECT_EQ(1.0, my_ray.get_direction().norm());
}