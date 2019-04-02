// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/primitive/Sphere.h"
namespace ml = merlict;


TEST_CASE("SphereTest: set_positiv_radius", "[merlict]") {
  ml::Sphere N;
  CHECK_NOTHROW(N.set_radius(1.0));
}

TEST_CASE("SphereTest: set_negativ_radius", "[merlict]") {
  ml::Sphere N;
  CHECK_THROWS_AS(N.set_radius(-1.0), ml::Sphere::BadRadius);
}

TEST_CASE("SphereTest: set_zero_radius", "[merlict]") {
  ml::Sphere N;
  CHECK_THROWS_AS(N.set_radius(0.0), ml::Sphere::BadRadius);
}
