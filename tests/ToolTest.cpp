// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "catch.hpp"
#include "Core/tools.h"
#include "Tools/FileTools.h"
using namespace relleums;



TEST_CASE("Tools: deg2rad_rad2deg", "[mctracer]") {
    for (int i = -721; i < 721; i++) {
        double angle_in_deg = static_cast<double>(i);
        CHECK(angle_in_deg == Approx(rad2deg(deg2rad(angle_in_deg))).margin(1e-9));
    }
}

TEST_CASE("Tools: deg2rad", "[mctracer]") {
    CHECK(0.0 == Approx(deg2rad(0.0)).margin(1e-9));
    CHECK(M_PI/2.0 == Approx(deg2rad(90.0)).margin(1e-9));
    CHECK(M_PI == Approx(deg2rad(180.0)).margin(1e-9));
    CHECK(-M_PI/2.0 == Approx(deg2rad(-90.0)).margin(1e-9));
    CHECK(-M_PI == Approx(deg2rad(-180.0)).margin(1e-9));
    CHECK(2.0*M_PI == Approx(deg2rad(360.0)).margin(1e-9));
}

TEST_CASE("Tools: rad2deg", "[mctracer]") {
    CHECK(0.0 == Approx(rad2deg(0.0*M_PI)).margin(1e-9));
    CHECK(45.0 == Approx(rad2deg(0.25*M_PI)).margin(1e-9));
    CHECK(90.0 == Approx(rad2deg(0.5*M_PI)).margin(1e-9));
    CHECK(180.0 == Approx(rad2deg(1.0*M_PI)).margin(1e-9));
    CHECK(180.0e3 == Approx(rad2deg(1.0e3*M_PI)).margin(1e-9));
}

TEST_CASE("Tools: odd_and_even", "[mctracer]") {
    CHECK(is_even(0));
    CHECK(is_even(2));
    CHECK(is_even(-2));
    CHECK(is_even(1214214));

    CHECK(is_odd(-1));
    CHECK(is_odd(-3));
    CHECK(is_odd(-7));
    CHECK(is_odd(-9));

    CHECK(!is_even(3));
    CHECK(!is_even(-1));
    CHECK(!is_even(1337));
    CHECK(!is_even(23));

    CHECK(!is_odd(2));
    CHECK(!is_odd(0));
    CHECK(!is_odd(42));
    CHECK(!is_odd(-46));
}

TEST_CASE("Tools: get_solid_angle_for_opening_angle", "[mctracer]") {
    CHECK(0.0 == Approx(get_solid_angle_for_opening_angle(0.0)).margin(1e-9));
    CHECK(2*M_PI == Approx(get_solid_angle_for_opening_angle(M_PI/2.0)).margin(1e-9));
    CHECK(4*M_PI == Approx(get_solid_angle_for_opening_angle(M_PI)).margin(1e-9));
}
