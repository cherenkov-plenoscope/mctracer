// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/primitive/Sphere.h"

using namespace merlict;



TEST_CASE("SphereTest: set_positiv_radius", "[mctracer]") {
  Sphere N;
  CHECK_NOTHROW(N.set_radius(1.0));
}

TEST_CASE("SphereTest: set_negativ_radius", "[mctracer]") {
  Sphere N;
  CHECK_THROWS_AS(N.set_radius(-1.0), Sphere::BadRadius);
}

TEST_CASE("SphereTest: set_zero_radius", "[mctracer]") {
  Sphere N;
  CHECK_THROWS_AS(N.set_radius(0.0), Sphere::BadRadius);
}
