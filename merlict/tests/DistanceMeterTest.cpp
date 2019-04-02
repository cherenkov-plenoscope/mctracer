// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/DistanceMeter.h"
#include "merlict/Ray.h"
#include "merlict/scenery/primitive/Sphere.h"
namespace ml = merlict;



TEST_CASE("DistanceMeterTest: is_facing", "[merlict]") {
    ml::Sphere sphere("sphere_1", ml::Vec3(0, 0, 0), ml::Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    ml::Ray ray(ml::Vec3(0, 0, -1), ml::Vec3(0, 0, 1));
    ml::DistanceMeter dist_meter(&ray, &sphere);

    CHECK(dist_meter.faces_an_object);
    CHECK(dist_meter.distance_to_closest_object == 0.5);
}

TEST_CASE("DistanceMeterTest: is_not_facing", "[merlict]") {
    ml::Sphere sphere("sphere_1", ml::Vec3(0, 0, 0), ml::Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    ml::Ray ray(ml::Vec3(0, 0, +1), ml::Vec3(0, 0, 1));
    ml::DistanceMeter dist_meter(&ray, &sphere);

    CHECK(!dist_meter.faces_an_object);
}
