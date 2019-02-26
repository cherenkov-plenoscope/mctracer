// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/mctracer.h"
using std::vector;

using namespace relleums;

struct SphereIntersectionTest {
    Scenery scenery;
    PropagationConfig setup;
    double radius;
    Sphere* my_sphere;
    random::Mt19937 prng;
    double wavelength = 433e-9;
    PropagationEnvironment env;

    SphereIntersectionTest() {
        prng.set_seed(0u);
        scenery.colors.add("col", Color(200, 128, 128));

        radius = 1.0;
        my_sphere = scenery.root.append<Sphere>();
        my_sphere->set_name_pos_rot("my_sphere", Vec3(0, 0, 0), Rot3(0, 0, 0));
        my_sphere->set_inner_color(scenery.colors.get("col"));
        my_sphere->set_outer_color(scenery.colors.get("col"));
        my_sphere->set_radius(radius);

        scenery.root.init_tree_based_on_mother_child_relations();

        env.root_frame = &scenery.root;
        env.config = &setup;
        env.prng = &prng;
    }
};

TEST_CASE("SphereIntersectionTest: frontal", "[mctracer]") {
    SphereIntersectionTest st;
    double x_pos = -5.0;
    Vec3 support(x_pos, 0.0, 0.0);
    Vec3 direction(1.0, 0.0, 0.0);
    Photon P(support , direction, st.wavelength);
    PhotonAndFrame::Propagator(&P, st.env);

    REQUIRE(2u == P.get_number_of_interactions_so_far());
    REQUIRE(-st.radius-x_pos == P.get_accumulative_distance());

    CHECK(P.get_intersection_at(1).surface_normal_in_object_frame() == VEC3_UNIT_X*(-1));
}

TEST_CASE("SphereIntersectionTest: emmitting_close_above_surface_tangential", "[mctracer]") {
    SphereIntersectionTest st;
    Vec3 support(0.0, 0.0, 1.0+1e-9);
    Vec3 direction(1.0, 0.0, 0.0);
    Photon P(support, direction, st.wavelength);
    PhotonAndFrame::Propagator(&P, st.env);
    REQUIRE(P.get_final_interaction_type() == absorption_in_void);
}

TEST_CASE("SphereIntersectionTest: emmitting_close_above_surface_straigtht_away", "[mctracer]") {
    SphereIntersectionTest st;
    Vec3 support(0.0, 0.0, 1.0+1e-9);
    Vec3 direction(0.0, 0.0, 1.0);
    Photon P(support, direction, st.wavelength);
    PhotonAndFrame::Propagator(&P, st.env);
    REQUIRE(P.get_final_interaction_type() == absorption_in_void);
}

TEST_CASE("SphereIntersectionTest: tangential_intersection", "[mctracer]") {
    SphereIntersectionTest st;
    Vec3 support(-5.0, 0.0, 1.0);
    Vec3 direction(1.0, 0.0, 0.0);
    Photon P(support, direction, st.wavelength);
    PhotonAndFrame::Propagator(&P, st.env);
    REQUIRE(P.get_number_of_interactions_so_far() == 2u);
    Vec3 normal = VEC3_UNIT_Z;
    CHECK(0.0 == Approx(normal.distance_to(P.get_intersection_at(1).surface_normal_in_object_frame())).margin(1e-12));
}

TEST_CASE("SphereIntersectionTest: ray_frontal_intersection", "[mctracer]") {
    SphereIntersectionTest st;
    Ray ray_with_intersection(Vec3(0.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0));
    vector<Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_with_intersection, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == Vec3(0.0, 0.0, -1.0));
    CHECK(intersections.front().surface_normal_in_object_frame() == Vec3(0.0, 0.0, -1.0));
}

TEST_CASE("SphereIntersectionTest: ray_intersection_but_no_causal_intersection", "[mctracer]") {
    SphereIntersectionTest st;
    Ray ray_without_intersection(Vec3(0.0, 0.0, +2.0), Vec3(0.0, 0.0, 1.0));
    vector<Intersection> intersections;
    st.my_sphere->calculate_intersection_with(
    &ray_without_intersection,
    &intersections);
    CHECK(intersections.empty());
}

TEST_CASE("SphereIntersectionTest: ray_completely_outside_of_sphere", "[mctracer]") {
    SphereIntersectionTest st;
    Ray ray_outside(Vec3(5.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0));
    vector<Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_outside, &intersections);

    CHECK(intersections.empty());
}

TEST_CASE("SphereIntersectionTest: ray_starts_inside_sphere", "[mctracer]") {
    SphereIntersectionTest st;
    Ray ray_inside(VEC3_ORIGIN, Vec3(0.0, 0.0, 1.0));
    vector<Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_inside, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == Vec3(0.0, 0.0, +1.0));
    CHECK(intersections.front().surface_normal_in_object_frame() == Vec3(0.0, 0.0, +1.0));
}

TEST_CASE("SphereIntersectionTest: ray_tangents_sphere", "[mctracer]") {
    SphereIntersectionTest st;
    Ray ray_inside(Vec3(1.0, 0.0, -2.0), Vec3(0.0, 0.0, 1.0));
    vector<Intersection> intersections;
    st.my_sphere->calculate_intersection_with(&ray_inside, &intersections);

    REQUIRE(!intersections.empty());
    CHECK(st.my_sphere == intersections.front().get_object());
    CHECK(intersections.front().position_in_object_frame() == Vec3(1.0, 0.0, 0.0));
    CHECK(intersections.front().surface_normal_in_object_frame() == Vec3(1.0, 0.0, 0.0));
}
