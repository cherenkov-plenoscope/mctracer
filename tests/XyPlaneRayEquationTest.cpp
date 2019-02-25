// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/scenery/geometry/HexagonalPrismZ.h"
#include "Core/scenery/geometry/XyPlaneRayIntersectionEquation.h"
#include "Core/Ray.h"

using namespace relleums;



TEST_CASE("XyPlaneRayEquationTest: frontal_hit", "[mctracer]") {
  for (unsigned int i = -100; i < 100; i++) {
    double sup_z = static_cast<double>(i)*1.337;
    Ray ray(Vec3(0.0, 0.0, sup_z), Vec3(0.0, 0.0, 1.0));

    XyPlaneRayIntersectionEquation eq(&ray);

    REQUIRE(eq.has_solution());
    CHECK(eq.get_ray_parameter_for_intersection() == sup_z);
    CHECK( == /* GTEST-CATCH FIXME */sup_z < 0.0, eq.has_causal_solution());
  }
}

TEST_CASE("XyPlaneRayEquationTest: no_hit_at_all", "[mctracer]") {
  Ray ray(Vec3(0.0, 0.0, 1.0), Vec3(1.0, 0.0, 0.0));
  XyPlaneRayIntersectionEquation eq(&ray);
  CHECK(!eq.has_solution());
}

TEST_CASE("XyPlaneRayEquationTest: always_ez_normal_vector", "[mctracer]") {
  Ray ray(Vec3(0.0, 0.0, 1.0), Vec3(1.0, 0.0, 0.0));
  XyPlaneRayIntersectionEquation eq(&ray);
  CHECK(eq.get_plane_normal_vector() == VEC3_UNIT_Z);
}
