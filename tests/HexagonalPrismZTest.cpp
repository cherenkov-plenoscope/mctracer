// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/scenery/geometry/HexagonalPrismZ.h"
namespace ml = merlict;


TEST_CASE("HexagonalPrismZTest: throw_when_negativ_radius", "[merlict]") {
    ml::HexagonalPrismZ hexBound;
    CHECK_THROWS_AS(hexBound.set_outer_radius(-55.0), std::invalid_argument);
}

TEST_CASE("HexagonalPrismZTest: throw_when_zero_radius", "[merlict]") {
    ml::HexagonalPrismZ hexBound;
    CHECK_THROWS_AS(hexBound.set_outer_radius(0.0), std::invalid_argument);
}

TEST_CASE("HexagonalPrismZTest: is_inside", "[merlict]") {
    double outer_radius = 1.0;
    double inner_radius = outer_radius * cos(M_PI/6.0);

    ml::HexagonalPrismZ hexBound;
    hexBound.set_outer_radius(1.0);

    ml::Vec3 vec(0.0, 0.0, 0.0);
    CHECK(hexBound.is_inside(&vec));

    vec.set(0.0, inner_radius-0.01, 0.0);
    CHECK(hexBound.is_inside(&vec));

    vec.set(0.0, inner_radius+0.01, 0.0);
    CHECK(!hexBound.is_inside(&vec));

    vec.set(outer_radius-0.01, 0.0, 0.0);
    CHECK(hexBound.is_inside(&vec));

    vec.set(outer_radius+0.01, 0.0, 0.0);
    CHECK(!hexBound.is_inside(&vec));

    vec.set(120.0, 5.0, 0.0);
    CHECK(!hexBound.is_inside(&vec));
}
