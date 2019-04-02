// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/ColorMap.h"
namespace ml = merlict;


TEST_CASE("ColorMapTest: init_empty", "[merlict]") {
    ml::ColorMap my_colors;
    CHECK(!my_colors.has("red"));
}

TEST_CASE("ColorMapTest: get_not_existing_color", "[merlict]") {
    ml::ColorMap my_colors;
    CHECK_THROWS_AS(my_colors.get("red"), ml::ColorMap::NoSuchKey);
}

TEST_CASE("ColorMapTest: add_color_and_check_it_is_in", "[merlict]") {
    ml::ColorMap my_colors;
    my_colors.add("red", ml::Color(255, 0, 0));
    CHECK(my_colors.has("red"));
}

TEST_CASE("ColorMapTest: add_color_and_add_it_again", "[merlict]") {
    ml::ColorMap my_colors;
    my_colors.add("red", ml::Color(255, 0, 0));
    CHECK_THROWS_AS(my_colors.add("red", ml::Color(128, 0, 0)), ml::ColorMap::KeyAlreadyInUse);
}

TEST_CASE("ColorMapTest: add_color_and_get_it_again", "[merlict]") {
    ml::ColorMap my_colors;
    ml::Color novel_red(255, 0, 0);
    my_colors.add("red", novel_red);
    const ml::Color* my_red = my_colors.get("red");
    CHECK(novel_red.r == my_red->r);
    CHECK(novel_red.g == my_red->g);
    CHECK(novel_red.b == my_red->b);
}

