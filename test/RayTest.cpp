#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Ray.h"
#include "Sphere.h"

using namespace std;

class RayTest : public ::testing::Test {
 protected:

  Vector3D support;
  Vector3D direction;

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

  EXPECT_EQ(support, my_ray.Support());
  EXPECT_EQ(direction/direction.norm(), my_ray.Direction()); 
  EXPECT_EQ(1.0, my_ray.Direction().norm());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, position_on_ray) {
  Ray my_ray(support, direction);

  double ray_parameter = 5.0;
  Vector3D pos_on_ray = support + direction*ray_parameter;
    
  EXPECT_EQ(pos_on_ray, my_ray.PositionOnRay(ray_parameter));
}
//------------------------------------------------------------------------------
TEST_F(RayTest, closest_distance_to_point) {
  Ray my_ray(Vector3D::null, Vector3D(0.0, 0.0, 1.0));

  EXPECT_EQ(0.0, my_ray.get_closest_distance_to_point(Vector3D::null));
  EXPECT_EQ(1.0, my_ray.get_closest_distance_to_point(Vector3D(1.0, 0.0, 0.0)));
  EXPECT_EQ(1.0, my_ray.get_closest_distance_to_point(Vector3D(0.0, 1.0, 0.0)));
  EXPECT_EQ(0.0, my_ray.get_closest_distance_to_point(Vector3D(0.0, 0.0, 1.0)));
}
//------------------------------------------------------------------------------
TEST_F(RayTest, set_direction) {
  Ray my_ray(support, direction*42.0);
  my_ray.SetDirection(Vector3D(1.2, 3.4, 5.6));
  EXPECT_EQ(1.0, my_ray.Direction().norm());
}