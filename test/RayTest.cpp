#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Ray.h"
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
  EXPECT_EQ(direction/direction.norm2(), my_ray.Direction()); 
  EXPECT_EQ(1.0, my_ray.Direction().norm2());
  EXPECT_EQ(nullptr, my_ray.GetHistory());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, set_Ray_using_vectors) {

  Ray my_ray;
  my_ray.SetRay(support, direction);

  EXPECT_EQ(support, my_ray.Support());
  EXPECT_EQ(direction/direction.norm2(), my_ray.Direction()); 
  EXPECT_EQ(1.0, my_ray.Direction().norm2());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, ID) {

  uint id = 1337;

  Ray my_ray;
  my_ray.SetID(id);

  EXPECT_EQ(id, my_ray.ID());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, set_support_using_xyz_components) {


  Ray my_ray;
  my_ray.SetSupport(support.x(), support.y(), support.z());

  EXPECT_EQ(support, my_ray.Support());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, set_direction_using_xyz_components) {

  Ray my_ray;
  my_ray.SetDirection(direction.x() ,direction.y(), direction.z());

  EXPECT_EQ(direction/direction.norm2(), my_ray.Direction());
}
//------------------------------------------------------------------------------
TEST_F(RayTest, position_on_ray) {
  Ray my_ray(support, direction);

  double ray_parameter = 5.0;
  Vector3D pos_on_ray = support + direction*ray_parameter;
    
  EXPECT_EQ(pos_on_ray, my_ray.PositionOnRay(ray_parameter));
}
//------------------------------------------------------------------------------