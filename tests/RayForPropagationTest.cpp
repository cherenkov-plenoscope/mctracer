// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/RayForPropagation.h"
#include "Core/SimulationTruth.h"

using namespace relleums;

TEST_CASE("RayForPropagationTest: Constructor_using_vectors", "[mctracer]") {
    Vec3 support(0.0, 0.0, 1.0);
    Vec3 direction(0.0, 0.0, 1.0);
    RayForPropagation prop_ray(support, direction);
    prop_ray.set_simulation_truth_id(1337);

    CHECK(prop_ray.support() == support);
    CHECK(prop_ray.direction() == direction);
    CHECK(prop_ray.get_number_of_interactions_so_far() == 1u);
    // creation itselt
    CHECK(prop_ray.get_simulation_truth_id() == 1337);
}

TEST_CASE("RayForPropagationTest: direction_normalized", "[mctracer]") {
    Vec3 support(0.0, 0.0, 1.0);
    Vec3 direction(0.0, 0.0, 1.0);
    RayForPropagation prop_ray(support, direction*42.0);
    CHECK(prop_ray.direction().norm() == 1.0);
}

TEST_CASE("RayForPropagationTest: distance_passed_after_creation", "[mctracer]") {
    Vec3 support(0.0, 0.0, 1.0);
    Vec3 direction(0.0, 0.0, 1.0);
    RayForPropagation prop_ray(support, direction);
    CHECK(prop_ray.get_accumulative_distance() == 0.0);
}

TEST_CASE("RayForPropagationTest: default_simulation_truth_id", "[mctracer]") {
    Vec3 support(0.0, 0.0, 1.0);
    Vec3 direction(0.0, 0.0, 1.0);
    RayForPropagation prop_ray(support, direction);
    CHECK(prop_ray.get_simulation_truth_id() == SimulationTruth::MCTRACER_DEFAULT);
}
