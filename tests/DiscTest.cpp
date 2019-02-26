// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/primitive/Disc.h"

using namespace relleums;



TEST_CASE("DiscTest: set_Disc", "[mctracer]") {
    Disc D;
    D.set_radius(42.0);
    CHECK(D.get_bounding_sphere_radius() == 42.0);
}

TEST_CASE("DiscTest: set_Disc_negative_radius", "[mctracer]") {
	Disc D;
    CHECK_THROWS_AS(D.set_radius(-42.0), std::invalid_argument);
}
