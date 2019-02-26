// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/ZaxisCylinderRayIntersectionEquation.h"

using namespace merlict;



TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_inside_run_in_z_no_hit", "[mctracer]") {
  Ray ray(VEC3_ORIGIN, VEC3_ORIGIN);

  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  CHECK(!eq.has_solution());
  CHECK(!eq.has_causal_solution());
}

TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_inside_and_hit", "[mctracer]") {
  Ray ray(VEC3_ORIGIN, Vec3(1.0, 0.0, 0.0));
  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  CHECK(eq.has_solution());
  CHECK(eq.one_intersec_behind_and_one_in_front_ray_sup());
  CHECK(!eq.both_intersects_in_front_ray_sup());
  CHECK(eq.has_causal_solution());
  CHECK(eq.get_ray_parameter_for_intersection() == 1.0);
}

TEST_CASE("ZaxisCylinderRayIntersectionEquationTest: start_outside_and_hit", "[mctracer]") {
  Ray ray(Vec3(-2.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0));
  ZaxisCylinderRayIntersectionEquation eq(1.0, &ray);

  CHECK(eq.has_solution());
  CHECK(eq.both_intersects_in_front_ray_sup());
  CHECK(!eq.one_intersec_behind_and_one_in_front_ray_sup());
  CHECK(eq.has_causal_solution());
  CHECK(eq.get_ray_parameter_for_intersection() == 1.0);
}
