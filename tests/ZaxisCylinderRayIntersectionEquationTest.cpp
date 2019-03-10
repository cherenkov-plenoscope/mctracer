// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/ZaxisCylinderRayIntersectionEquation.h"
namespace ml = merlict;


TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_inside_run_in_z_no_hit", "[merlict]") {
  ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_ORIGIN);

  ml::ZaxisCylinderRayIntersectionEquation eq(1., &ray);

  CHECK(!eq.has_solution());
  CHECK(!eq.has_causal_solution());
}

TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_inside_and_hit", "[merlict]") {
  ml::Ray ray(ml::VEC3_ORIGIN, ml::Vec3(1, 0, 0));
  ml::ZaxisCylinderRayIntersectionEquation eq(1., &ray);

  CHECK(eq.has_solution());
  CHECK(eq.one_intersec_behind_and_one_in_front_ray_sup());
  CHECK(!eq.both_intersects_in_front_ray_sup());
  CHECK(eq.has_causal_solution());
  CHECK(eq.get_ray_parameter_for_intersection() == 1.);
}

TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_outside_and_hit", "[merlict]") {
  ml::Ray ray(ml::Vec3(-2, 0, 0), ml::Vec3(1, 0, 0));
  ml::ZaxisCylinderRayIntersectionEquation eq(1., &ray);

  CHECK(eq.has_solution());
  CHECK(eq.both_intersects_in_front_ray_sup());
  CHECK(!eq.one_intersec_behind_and_one_in_front_ray_sup());
  CHECK(eq.has_causal_solution());
  CHECK(eq.get_ray_parameter_for_intersection() == 1.);
}
