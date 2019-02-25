// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/DistanceMeter.h"
#include "Core/Ray.h"
#include "Core/scenery/primitive/Sphere.h"

using namespace relleums;



TEST_CASE("DistanceMeterTest: is_facing", "[mctracer]") {
    Sphere sphere = Sphere("sphere_1", Vec3(0, 0, 0), Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    Ray ray = Ray(Vec3(0, 0, -1), Vec3(0, 0, 1));
    DistanceMeter dist_meter = DistanceMeter(&ray, &sphere);

    CHECK(dist_meter.faces_an_object);
    CHECK(dist_meter.distance_to_closest_object == 0.5);
}

TEST_CASE("DistanceMeterTest: is_not_facing", "[mctracer]") {
    Sphere sphere = Sphere("sphere_1", Vec3(0, 0, 0), Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    Ray ray = Ray(Vec3(0, 0, +1), Vec3(0, 0, 1));
    DistanceMeter dist_meter = DistanceMeter(&ray, &sphere);

    CHECK(!dist_meter.faces_an_object);
}
