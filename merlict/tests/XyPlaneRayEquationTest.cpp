// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/HexagonalPrismZ.h"
#include "merlict/scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "merlict/Ray.h"
namespace ml = merlict;


TEST_CASE("XyPlaneRayEquationTest: frontal_hit", "[merlict]") {
  for (unsigned int i = -100; i < 100; i++) {
    double sup_z = static_cast<double>(i)*1.337;
    ml::Ray ray(ml::Vec3(0, 0, sup_z), ml::Vec3(0, 0, 1));

    ml::XyPlaneRayIntersectionEquation eq(&ray);

    REQUIRE(eq.has_solution());
    CHECK(eq.get_ray_parameter_for_intersection() == sup_z);
    CHECK(eq.has_causal_solution());
  }
}

TEST_CASE("XyPlaneRayEquationTest: no_hit_at_all", "[merlict]") {
  ml::Ray ray(ml::Vec3(0, 0, 1), ml::Vec3(1, 0, 0));
  ml::XyPlaneRayIntersectionEquation eq(&ray);
  CHECK_FALSE(eq.has_solution());
}

TEST_CASE("XyPlaneRayEquationTest: always_ez_normal_vector", "[merlict]") {
  ml::Ray ray(ml::Vec3(0, 0, 1), ml::Vec3(1, 0, 0));
  ml::XyPlaneRayIntersectionEquation eq(&ray);
  CHECK(eq.get_plane_normal_vector() == ml::VEC3_UNIT_Z);
}
