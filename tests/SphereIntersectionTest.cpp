// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


struct SphereIntersectionTest {
    ml::Scenery scenery;
    ml::PropagationConfig setup;
    double radius;
    ml::Sphere* my_sphere;
    ml::random::Mt19937 prng;
    double wavelength = 433e-9;
    ml::PropagationEnvironment env;

    SphereIntersectionTest() {
        prng.set_seed(0u);
        scenery.colors.add("col", ml::Color(200, 128, 128));

        radius = 1;
        my_sphere = scenery.root.add<ml::Sphere>();
        my_sphere->set_name_pos_rot("my_sphere", ml::Vec3(0, 0, 0), ml::Rot3(0, 0, 0));
        my_sphere->set_inner_color(scenery.colors.get("col"));
        my_sphere->set_outer_color(scenery.colors.get("col"));
        my_sphere->set_radius(radius);

        scenery.root.init_tree_based_on_mother_child_relations();

        env.root_frame = &scenery.root;
        env.config = &setup;
        env.prng = &prng;
    }
};

TEST_CASE("SphereIntersectionTest: frontal", "[merlict]") {
    SphereIntersectionTest st;
    double x_pos = -5;
    ml::Vec3 support(x_pos, 0, 0);
    ml::Vec3 direction(1, 0, 0);
    ml::Photon P(support , direction, st.wavelength);
    ml::Propagator(&P, st.env);

    REQUIRE(2u == P.get_num_interactions_so_far());
    REQUIRE(-st.radius-x_pos == P.accumulated_distance());

    CHECK(P.get_intersection_at(1).surface_normal_in_object_frame() == ml::VEC3_UNIT_X*(-1));
}

TEST_CASE("SphereIntersectionTest: emmitting_close_above_surface_tangential", "[merlict]") {
    SphereIntersectionTest st;
    ml::Vec3 support(0, 0, 1+1e-9);
    ml::Vec3 direction(1, 0, 0);
    ml::Photon P(support, direction, st.wavelength);
    ml::Propagator(&P, st.env);
    REQUIRE(P.get_final_interaction_type() == ml::absorption_in_void);
}

TEST_CASE("SphereIntersectionTest: emmitting_close_above_surface_straigtht_away", "[merlict]") {
    SphereIntersectionTest st;
    ml::Vec3 support(0, 0, 1+1e-9);
    ml::Vec3 direction(0, 0, 1);
    ml::Photon P(support, direction, st.wavelength);
    ml::Propagator(&P, st.env);
    REQUIRE(P.get_final_interaction_type() == ml::absorption_in_void);
}

TEST_CASE("SphereIntersectionTest: tangential_intersection", "[merlict]") {
    SphereIntersectionTest st;
    ml::Vec3 support(-5, 0, 1);
    ml::Vec3 direction(1, 0, 0);
    ml::Photon P(support, direction, st.wavelength);
    ml::Propagator(&P, st.env);
    REQUIRE(P.get_num_interactions_so_far() == 2u);
    ml::Vec3 normal = ml::VEC3_UNIT_Z;
    CHECK(0 == Approx(normal.distance_to(P.get_intersection_at(1).surface_normal_in_object_frame())).margin(1e-12));
}

TEST_CASE("SphereIntersectionTest: ray_frontal_intersection", "[merlict]") {
    SphereIntersectionTest st;
    ml::Ray ray_with_intersection(ml::Vec3(0, 0, -2), ml::Vec3(0, 0, 1));
    std::vector<ml::Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_with_intersection, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == ml::Vec3(0, 0, -1));
    CHECK(intersections.front().surface_normal_in_object_frame() == ml::Vec3(0, 0, -1));
}

TEST_CASE("SphereIntersectionTest: ray_intersection_but_no_causal_intersection", "[merlict]") {
    SphereIntersectionTest st;
    ml::Ray ray_without_intersection(ml::Vec3(0, 0, +2), ml::Vec3(0, 0, 1));
    std::vector<ml::Intersection> intersections;
    st.my_sphere->calculate_intersection_with(
    &ray_without_intersection,
    &intersections);
    CHECK(intersections.empty());
}

TEST_CASE("SphereIntersectionTest: ray_completely_outside_of_sphere", "[merlict]") {
    SphereIntersectionTest st;
    ml::Ray ray_outside(ml::Vec3(5, 0, 0), ml::Vec3(0, 0, 1));
    std::vector<ml::Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_outside, &intersections);

    CHECK(intersections.empty());
}

TEST_CASE("SphereIntersectionTest: ray_starts_inside_sphere", "[merlict]") {
    SphereIntersectionTest st;
    ml::Ray ray_inside(ml::VEC3_ORIGIN, ml::Vec3(0, 0, 1));
    std::vector<ml::Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_inside, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == ml::Vec3(0, 0, +1));
    CHECK(intersections.front().surface_normal_in_object_frame() == ml::Vec3(0, 0, +1));
}

TEST_CASE("SphereIntersectionTest: ray_tangents_sphere", "[merlict]") {
    SphereIntersectionTest st;
    ml::Ray ray_inside(ml::Vec3(1, 0, -2), ml::Vec3(0, 0, 1));
    std::vector<ml::Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_inside, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == ml::Vec3(1, 0, 0));
    CHECK(intersections.front().surface_normal_in_object_frame() == ml::Vec3(1, 0, 0));
}
