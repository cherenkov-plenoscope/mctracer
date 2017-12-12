// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/Ray.h"
#include "Scenery/Primitive/Sphere.h"

class RayTest : public ::testing::Test {};

TEST_F(RayTest, Constructor_using_vectors) {
  const Vec3 sup(1.2, 3.4, 5.6);
  const Vec3 dir(7.8, 9.0, 4.2);
  const Ray ray(sup, dir);
  EXPECT_EQ(sup, ray.support());
  EXPECT_EQ(dir/dir.norm(), ray.direction());
  EXPECT_NEAR(1.0, ray.direction().norm(), 1e-6);
}

TEST_F(RayTest, position_on_ray) {
  const Vec3 sup(1.2, 3.4, 5.6);
  const Vec3 dir(7.0, 9.0, 4.0);
  const Ray ray(sup, dir);
  const double alpha = 5.0;
  const Vec3 pos_on_ray = sup+dir/dir.norm()*alpha;
  EXPECT_NEAR(0.0, (pos_on_ray - ray.position_at(alpha) ).norm(), 1e-6);
}

TEST_F(RayTest, closest_distance_to_point) {
  Ray ray(Vec3::ORIGIN, Vec3(0.0, 0.0, 1.0));
  EXPECT_EQ(0.0, ray.closest_distance_to_point(Vec3::ORIGIN));
  EXPECT_EQ(1.0, ray.closest_distance_to_point(Vec3(1.0, 0.0, 0.0)));
  EXPECT_EQ(1.0, ray.closest_distance_to_point(Vec3(0.0, 1.0, 0.0)));
  EXPECT_EQ(0.0, ray.closest_distance_to_point(Vec3(0.0, 0.0, 1.0)));
}

TEST_F(RayTest, set_support_and_direction) {
  Ray ray(Vec3(1.2, 3.4, 5.6), Vec3(0, 0, 1));
  ray.set_support_and_direction(Vec3(1.2, 3.4, 5.6), Vec3(1.2, 3.4, 5.6));
  EXPECT_EQ(1.0, ray.direction().norm());
}
