// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/Ray.h"
#include "merlict/scenery/primitive/Sphere.h"

using namespace merlict;



TEST_CASE("RayTest: Constructor_using_vectors", "[mctracer]") {
  const Vec3 sup(1.2, 3.4, 5.6);
  const Vec3 dir(7.8, 9.0, 4.2);
  const Ray ray(sup, dir);
  CHECK(ray.support() == sup);
  CHECK(ray.direction() == dir/dir.norm());
  CHECK(1.0 == Approx(ray.direction().norm()).margin(1e-6));
}

TEST_CASE("RayTest: position_on_ray", "[mctracer]") {
  const Vec3 sup(1.2, 3.4, 5.6);
  const Vec3 dir(7.0, 9.0, 4.0);
  const Ray ray(sup, dir);
  const double alpha = 5.0;
  const Vec3 pos_on_ray = sup+dir/dir.norm()*alpha;
  CHECK(0.0 == Approx((pos_on_ray - ray.position_at(alpha) ).norm()).margin(1e-6));
}

TEST_CASE("RayTest: closest_distance_to_point", "[mctracer]") {
  Ray ray(VEC3_ORIGIN, Vec3(0.0, 0.0, 1.0));
  CHECK(ray.closest_distance_to_point(VEC3_ORIGIN) == 0.0);
  CHECK(ray.closest_distance_to_point(Vec3(1.0, 0.0, 0.0)) == 1.0);
  CHECK(ray.closest_distance_to_point(Vec3(0.0, 1.0, 0.0)) == 1.0);
  CHECK(ray.closest_distance_to_point(Vec3(0.0, 0.0, 1.0)) == 0.0);
}

TEST_CASE("RayTest: set_support_and_direction", "[mctracer]") {
  Ray ray(Vec3(1.2, 3.4, 5.6), Vec3(0, 0, 1));
  ray.set_support_and_direction(Vec3(1.2, 3.4, 5.6), Vec3(1.2, 3.4, 5.6));
  CHECK(ray.direction().norm() == 1.0);
}
