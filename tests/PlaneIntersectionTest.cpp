// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;

struct PlaneIntersectionTest {
    Scenery scenery;
    function::Func1* refl_vs_wavl;
    double x_width = 2.5;
    double y_width = 1.3;
    Plane* plane;
    random::Mt19937 prng;
    double wavelength = 433e-9;

    PlaneIntersectionTest() {
        prng.set_seed(0u);
        scenery.functions.add("refl_vs_wavl", function::Func1(
            {
                {200e-9, 1.0},
                {1200e-9, 1.0}
            }));

        plane = scenery.root.append<Plane>();
        plane->set_name_pos_rot("My_Plane", Vec3(0, 0, 0), Rot3(0, 0, 0));
        plane->set_outer_reflection(scenery.functions.get("refl_vs_wavl"));
        plane->set_inner_reflection(scenery.functions.get("refl_vs_wavl"));
        plane->set_x_y_width(x_width, y_width);

        scenery.root.init_tree_based_on_mother_child_relations();
    }
};

TEST_CASE("PlaneIntersectionTest: frontal", "[merlict]") {
    PlaneIntersectionTest pt;
    Ray ray(Vec3(0.0, 0.0, -1.0), Vec3(0.0, 0.0, 1.0));
    const Intersection intersec = RayAndFrame::first_intersection(
        &ray,
        &pt.scenery.root);

    REQUIRE(intersec.does_intersect());
    CHECK(intersec.get_object() == pt.plane);
    CHECK(intersec.position_in_object_frame() == VEC3_ORIGIN);
    CHECK(intersec.surface_normal_in_object_frame() == Vec3(0.0, 0.0, 1.0));
}

TEST_CASE("PlaneIntersectionTest: frontal_lateral_offset_alwas_intersection", "[merlict]") {
    PlaneIntersectionTest pt;
    for (int x_offset = -10; x_offset < 10; x_offset++) {
        for (int y_offset = -10; y_offset < 10; y_offset++) {
            double x_support = static_cast<double>(x_offset)*0.01;
            double y_support = static_cast<double>(y_offset)*0.01;

            Ray ray(Vec3(x_support, y_support, -1.0), Vec3(0.0, 0.0, 1.0));
            const Intersection intersec = RayAndFrame::first_intersection(
                &ray,
                &pt.scenery.root);

            REQUIRE(intersec.does_intersect());
            CHECK(intersec.get_object() == pt.plane);
            CHECK(intersec.position_in_object_frame() == Vec3(x_support, y_support, 0.0));
            CHECK(intersec.surface_normal_in_object_frame() == Vec3(0.0, 0.0, 1.0));
        }
    }
}

TEST_CASE("PlaneIntersectionTest: close_miss_x", "[merlict]") {
    PlaneIntersectionTest pt;
    Ray ray(Vec3(pt.x_width/2.0+0.01, 0.0, -1.0), Vec3(0.0, 0.0, 1.0));
    const Intersection intersec = RayAndFrame::first_intersection(
        &ray,
        &pt.scenery.root);
    CHECK(!intersec.does_intersect());
}

TEST_CASE("PlaneIntersectionTest: close_miss_y", "[merlict]") {
    PlaneIntersectionTest pt;
    Ray ray(Vec3(0.0, pt.y_width/2.0+0.01, -1.0), Vec3(0.0, 0.0, 1.0));
    const Intersection intersec = RayAndFrame::first_intersection(
        &ray,
        &pt.scenery.root);
    CHECK(!intersec.does_intersect());
}

TEST_CASE("PlaneIntersectionTest: move_plane_up", "[merlict]") {
    PlaneIntersectionTest pt;

    Plane* plane2 = pt.scenery.root.append<Plane>();
    plane2->set_name_pos_rot("My_second_plane", Vec3(0, 0, 1), Rot3(0, 0, 0));
    plane2->set_outer_reflection(pt.scenery.functions.get("refl_vs_wavl"));
    plane2->set_inner_reflection(pt.scenery.functions.get("refl_vs_wavl"));
    plane2->set_x_y_width(pt.x_width, pt.y_width);

    pt.scenery.root.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3(0.0, 0.0, -1.0), Vec3(0.0, 0.0, 1.0));
    const Intersection intersec = RayAndFrame::first_intersection(
        &ray,
        &pt.scenery.root);
    CHECK(intersec.does_intersect());
}
