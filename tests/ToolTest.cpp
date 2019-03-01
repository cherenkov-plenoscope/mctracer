// Copyright 2014 Sebastian A. Mueller
#include <cmath>
#include "catch.hpp"
#include "merlict/tools.h"
namespace ml = merlict;


TEST_CASE("Tools: deg2rad_rad2deg", "[merlict]") {
    for (int i = -721; i < 721; i++) {
        double angle_in_deg = static_cast<double>(i);
        CHECK(angle_in_deg == Approx(ml::rad2deg(ml::deg2rad(angle_in_deg))).margin(1e-9));
    }
}

TEST_CASE("Tools: ml::deg2rad", "[merlict]") {
    CHECK(0.0 == Approx(ml::deg2rad(0.0)).margin(1e-9));
    CHECK(M_PI/2.0 == Approx(ml::deg2rad(90.0)).margin(1e-9));
    CHECK(M_PI == Approx(ml::deg2rad(180.0)).margin(1e-9));
    CHECK(-M_PI/2.0 == Approx(ml::deg2rad(-90.0)).margin(1e-9));
    CHECK(-M_PI == Approx(ml::deg2rad(-180.0)).margin(1e-9));
    CHECK(2.0*M_PI == Approx(ml::deg2rad(360.0)).margin(1e-9));
}

TEST_CASE("Tools: ml::rad2deg", "[merlict]") {
    CHECK(0.0 == Approx(ml::rad2deg(0.0*M_PI)).margin(1e-9));
    CHECK(45.0 == Approx(ml::rad2deg(0.25*M_PI)).margin(1e-9));
    CHECK(90.0 == Approx(ml::rad2deg(0.5*M_PI)).margin(1e-9));
    CHECK(180.0 == Approx(ml::rad2deg(1.0*M_PI)).margin(1e-9));
    CHECK(180.0e3 == Approx(ml::rad2deg(1.0e3*M_PI)).margin(1e-9));
}

TEST_CASE("Tools: odd_and_even", "[merlict]") {
    CHECK(ml::is_even(0));
    CHECK(ml::is_even(2));
    CHECK(ml::is_even(-2));
    CHECK(ml::is_even(1214214));

    CHECK(ml::is_odd(-1));
    CHECK(ml::is_odd(-3));
    CHECK(ml::is_odd(-7));
    CHECK(ml::is_odd(-9));

    CHECK(!ml::is_even(3));
    CHECK(!ml::is_even(-1));
    CHECK(!ml::is_even(1337));
    CHECK(!ml::is_even(23));

    CHECK(!ml::is_odd(2));
    CHECK(!ml::is_odd(0));
    CHECK(!ml::is_odd(42));
    CHECK(!ml::is_odd(-46));
}

TEST_CASE("Tools: get_solid_angle_for_opening_angle", "[merlict]") {
    CHECK(0.0 == Approx(ml::get_solid_angle_for_opening_angle(0.0)).margin(1e-9));
    CHECK(2*M_PI == Approx(ml::get_solid_angle_for_opening_angle(M_PI/2.0)).margin(1e-9));
    CHECK(4*M_PI == Approx(ml::get_solid_angle_for_opening_angle(M_PI)).margin(1e-9));
}
