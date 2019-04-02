// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/DualSphericalPrismZ.h"
namespace ml = merlict;


TEST_CASE("DualSphericalPrismZTest: is_inside", "[merlict]") {
    ml::DualSphericalPrismZ bound;
    bound.set_x_hight_and_y_width(1.0, 2.0);
    ml::Vec3 v(0.0, 0.0, 0.0);
    CHECK(bound.is_inside(&v));
    v.set(0.0, 0.0, 1.0);
    CHECK(bound.is_inside(&v));
    v.set(0.0, 0.0, -1.0);
    CHECK(bound.is_inside(&v));
    v.set(0.0, +0.99, 0.0);
    CHECK(bound.is_inside(&v));
    v.set(0.0, -0.99, 0.0);
    CHECK(bound.is_inside(&v));
    v.set(+0.49, 0.0, 0.0);
    CHECK(bound.is_inside(&v));
    v.set(-0.49, 0.0, 0.0);
    CHECK(bound.is_inside(&v));
}

TEST_CASE("DualSphericalPrismZTest: is_outside", "[merlict]") {
    ml::DualSphericalPrismZ bound;
    bound.set_x_hight_and_y_width(1.0, 2.0);
    ml::Vec3 v;
    v.set(+0.49, +0.99, 0.0);
    CHECK(!bound.is_inside(&v));
    v.set(+0.49, -0.99, 0.0);
    CHECK(!bound.is_inside(&v));
    v.set(-0.49, +0.99, 0.0);
    CHECK(!bound.is_inside(&v));
    v.set(-0.49, -0.99, 0.0);
    CHECK(!bound.is_inside(&v));
}
