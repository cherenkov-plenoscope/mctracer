// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/SensorMap.h"
using std::string;
using std::vector;
using namespace merlict;



TEST_CASE("SensorMapTest: init_one", "[mctracer]") {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    CHECK(1u == smap.sensors.size());
}

TEST_CASE("SensorMapTest: init_two", "[mctracer]") {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    smap.add(1u, &hans);
    CHECK(2u == smap.sensors.size());
}

TEST_CASE("SensorMapTest: add_zeroth_with_id_uneq_zero", "[mctracer]") {
    SensorMap smap;
    Frame hans;
    CHECK_THROWS_AS(smap.add(1u, &hans), std::invalid_argument);
}

TEST_CASE("SensorMapTest: add_first_with_id_uneq_one", "[mctracer]") {
    SensorMap smap;
    Frame hans;
    smap.add(0u, &hans);
    CHECK_THROWS_AS(smap.add(2u, &hans), std::invalid_argument);
}
