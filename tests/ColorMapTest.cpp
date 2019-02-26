// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/ColorMap.h"

using namespace merlict;



TEST_CASE("ColorMapTest: init_empty", "[merlict]") {
    ColorMap my_colors;
    CHECK(!my_colors.has("red"));
}

TEST_CASE("ColorMapTest: get_not_existing_color", "[merlict]") {
    ColorMap my_colors;
    CHECK_THROWS_AS(my_colors.get("red"), ColorMap::NoSuchKey);
}

TEST_CASE("ColorMapTest: add_color_and_check_it_is_in", "[merlict]") {
    ColorMap my_colors;
    my_colors.add("red", Color(255, 0, 0));
    CHECK(my_colors.has("red"));
}

TEST_CASE("ColorMapTest: add_color_and_add_it_again", "[merlict]") {
    ColorMap my_colors;
    my_colors.add("red", Color(255, 0, 0));
    CHECK_THROWS_AS(my_colors.add("red", Color(128, 0, 0)), ColorMap::KeyAlreadyInUse);
}

TEST_CASE("ColorMapTest: add_color_and_get_it_again", "[merlict]") {
    ColorMap my_colors;
    Color novel_red =  Color(255, 0, 0);
    my_colors.add("red", novel_red);
    const Color* my_red = my_colors.get("red");
    CHECK(novel_red.r == my_red->r);
    CHECK(novel_red.g == my_red->g);
    CHECK(novel_red.b == my_red->b);
}

