// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


struct PlaneIntersectionTest {
    ml::Scenery scenery;
    ml::function::Func1* refl_vs_wavl;
    double x_width = 2.5;
    double y_width = 1.3;
    ml::Plane* plane;
    ml::random::Mt19937 prng;
    double wavelength = 433e-9;

    PlaneIntersectionTest() {
        prng.set_seed(0u);
        scenery.functions.add("refl_vs_wavl", ml::function::Func1(
            {
                {200e-9, 1.0},
                {1200e-9, 1.0}
            }));

        plane = scenery.root.add<ml::Plane>();
        plane->set_name_pos_rot("plane", ml::Vec3(0, 0, 0), ml::Rot3(0, 0, 0));
        plane->outer_reflection = scenery.functions.get("refl_vs_wavl");
        plane->inner_reflection = scenery.functions.get("refl_vs_wavl");
        plane->set_x_y_width(x_width, y_width);

        scenery.root.init_tree_based_on_mother_child_relations();
    }
};

TEST_CASE("PlaneIntersectionTest: frontal", "[merlict]") {
    PlaneIntersectionTest pt;
    ml::Ray ray(ml::Vec3(0.0, 0.0, -1.0), ml::Vec3(0.0, 0.0, 1.0));
    const ml::Intersection intersec = ml::rays_first_intersection_with_frame(
        &ray,
        &pt.scenery.root);

    REQUIRE(intersec.does_intersect());
    CHECK(intersec.get_object() == pt.plane);
    CHECK(intersec.position_in_object_frame() == ml::VEC3_ORIGIN);
    CHECK(intersec.surface_normal_in_object_frame() == ml::Vec3(0, 0, 1));
}

TEST_CASE("PlaneIntersectionTest: frontal_lateral_offset_alwas_intersection", "[merlict]") {
    PlaneIntersectionTest pt;
    for (int x_offset = -10; x_offset < 10; x_offset++) {
        for (int y_offset = -10; y_offset < 10; y_offset++) {
            double x_support = static_cast<double>(x_offset)*0.01;
            double y_support = static_cast<double>(y_offset)*0.01;

            ml::Ray ray(ml::Vec3(x_support, y_support, -1), ml::Vec3(0, 0, 1));
            const ml::Intersection intersec = ml::rays_first_intersection_with_frame(
                &ray,
                &pt.scenery.root);

            REQUIRE(intersec.does_intersect());
            CHECK(intersec.get_object() == pt.plane);
            CHECK(intersec.position_in_object_frame() == ml::Vec3(x_support, y_support, 0));
            CHECK(intersec.surface_normal_in_object_frame() == ml::Vec3(0, 0, 1));
        }
    }
}

TEST_CASE("PlaneIntersectionTest: close_miss_x", "[merlict]") {
    PlaneIntersectionTest pt;
    ml::Ray ray(ml::Vec3(pt.x_width/2.0+0.01, 0, -1), ml::Vec3(0, 0, 1));
    const ml::Intersection intersec = ml::rays_first_intersection_with_frame(
        &ray,
        &pt.scenery.root);
    CHECK(!intersec.does_intersect());
}

TEST_CASE("PlaneIntersectionTest: close_miss_y", "[merlict]") {
    PlaneIntersectionTest pt;
    ml::Ray ray(ml::Vec3(0, pt.y_width/2.0+0.01, -1), ml::Vec3(0, 0, 1));
    const ml::Intersection intersec = ml::rays_first_intersection_with_frame(
        &ray,
        &pt.scenery.root);
    CHECK(!intersec.does_intersect());
}

TEST_CASE("PlaneIntersectionTest: move_plane_up", "[merlict]") {
    PlaneIntersectionTest pt;

    ml::Plane* plane2 = pt.scenery.root.add<ml::Plane>();
    plane2->set_name_pos_rot("plane2", ml::Vec3(0, 0, 1), ml::Rot3(0, 0, 0));
    plane2->outer_reflection = pt.scenery.functions.get("refl_vs_wavl");
    plane2->inner_reflection = pt.scenery.functions.get("refl_vs_wavl");
    plane2->set_x_y_width(pt.x_width, pt.y_width);

    pt.scenery.root.init_tree_based_on_mother_child_relations();

    ml::Ray ray(ml::Vec3(0, 0, -1), ml::Vec3(0, 0, 1));
    const ml::Intersection intersec = ml::rays_first_intersection_with_frame(
        &ray,
        &pt.scenery.root);
    CHECK(intersec.does_intersect());
}
