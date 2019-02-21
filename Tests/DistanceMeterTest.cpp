// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/DistanceMeter.h"
#include "Core/Ray.h"
#include "Scenery/Primitive/Sphere.h"

using namespace relleums;

class DistanceMeterTest : public ::testing::Test {};

TEST_F(DistanceMeterTest, is_facing) {
    Sphere sphere = Sphere("sphere_1", Vec3(0, 0, 0), Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    Ray ray = Ray(Vec3(0, 0, -1), Vec3(0, 0, 1));
    DistanceMeter dist_meter = DistanceMeter(&ray, &sphere);

    EXPECT_TRUE(dist_meter.faces_an_object);
    EXPECT_TRUE(dist_meter.distance_to_closest_object == 0.5);
}

TEST_F(DistanceMeterTest, is_not_facing) {
    Sphere sphere = Sphere("sphere_1", Vec3(0, 0, 0), Rot3(0, 0, 0));
    sphere.set_radius(0.5);
    Ray ray = Ray(Vec3(0, 0, +1), Vec3(0, 0, 1));
    DistanceMeter dist_meter = DistanceMeter(&ray, &sphere);

    EXPECT_FALSE(dist_meter.faces_an_object);
}
