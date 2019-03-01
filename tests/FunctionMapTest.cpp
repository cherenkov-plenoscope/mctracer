// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/FunctionMap.h"
namespace ml = merlict;


TEST_CASE("FunctionMapTest: init_empty", "[merlict]") {
    ml::FunctionMap my_funcs;
    CHECK(!my_funcs.has("mirror_reflectivity_vs_wavelength"));
}

TEST_CASE("FunctionMapTest: get_not_existing_color", "[merlict]") {
    ml::FunctionMap my_funcs;
    CHECK_THROWS_AS(my_funcs.get("f(x)"), ml::FunctionMap::NoSuchKey);
}

TEST_CASE("FunctionMapTest: add_func_and_check_it_is_in", "[merlict]") {
    ml::FunctionMap my_funcs;
    my_funcs.add("red", ml::function::Func1({{0, 0}, {1, 0}}));
    CHECK(my_funcs.has("red"));
}

TEST_CASE("FunctionMapTest: add_func_and_add_it_again", "[merlict]") {
    ml::FunctionMap my_funcs;
    my_funcs.add("f_vs_x", ml::function::Func1({{0, 0}, {1, 0}}));
    CHECK_THROWS_AS(my_funcs.add("f_vs_x", ml::function::Func1()), ml::FunctionMap::KeyAlreadyInUse);
}

TEST_CASE("FunctionMapTest: add_func_and_get_it_again", "[merlict]") {
    ml::FunctionMap my_funcs;
    my_funcs.add("f_vs_x", ml::function::Func1({{0, 0}, {1, 0}}));
    const ml::function::Func1* function = my_funcs.get("f_vs_x");
    CHECK(0 == function->limits.lower);
    CHECK(1 == function->limits.upper);
}

