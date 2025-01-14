// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/Color.h"
namespace ml = merlict;


TEST_CASE("ColorTest: default_ctor", "[merlict]") {
    ml::Color c;
    CHECK(0 == c.r);
    CHECK(0 == c.g);
    CHECK(0 == c.b);
}

TEST_CASE("ColorTest: reflection_mix", "[merlict]") {
    ml::Color base(0, 0, 0);
    ml::Color green(0, 100, 0);
    const double reflectivity = 0.5;
    base.reflection_mix(green, reflectivity);
    CHECK(0 == base.r);
    CHECK(50 == base.g);
    CHECK(0 == base.b);
}

TEST_CASE("ColorTest: ctor_mix", "[merlict]") {
    std::vector<ml::Color> rainbow;
    rainbow.push_back(ml::Color(100, 0, 0));
    rainbow.push_back(ml::Color(0, 100, 0));
    rainbow.push_back(ml::Color(0, 0, 100));
    ml::Color mix(rainbow);
    CHECK(mix.r == Approx(33).margin(1));
    CHECK(mix.g == Approx(33).margin(1));
    CHECK(mix.b == Approx(33).margin(1));
}
