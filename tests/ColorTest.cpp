// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/Color.h"

using namespace relleums;



TEST_CASE("ColorTest: default_ctor", "[mctracer]") {
    Color c;
    CHECK(0 == c.r);
    CHECK(0 == c.g);
    CHECK(0 == c.b);
}

TEST_CASE("ColorTest: reflection_mix", "[mctracer]") {
    Color base(0, 0, 0);
    Color green(0, 100, 0);
    const double reflectivity = 0.5;
    base.reflection_mix(green, reflectivity);
    CHECK(0 == base.r);
    CHECK(50 == base.g);
    CHECK(0 == base.b);
}

TEST_CASE("ColorTest: ctor_mix", "[mctracer]") {
    std::vector<Color> rainbow;
    rainbow.push_back(Color(100, 0, 0));
    rainbow.push_back(Color(0, 100, 0));
    rainbow.push_back(Color(0, 0, 100));
    Color mix = Color(rainbow);
    CHECK(mix.r == Approx(33).margin(1));
    CHECK(mix.g == Approx(33).margin(1));
    CHECK(mix.b == Approx(33).margin(1));
}
