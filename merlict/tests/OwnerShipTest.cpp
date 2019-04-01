// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include <string>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("OwnerShipTest: own", "[merlict]") {
    ml::Scenery scenery;
    scenery.colors.add("green", ml::COLOR_GREEN);
    scenery.colors.add("red", ml::COLOR_RED);
    scenery.colors.add("grass_green", ml::COLOR_GRASS_GREEN);

    ml::Sphere* sphere = scenery.root.add<ml::Sphere>();
    sphere->set_name_pos_rot("leaf_ball", ml::Vec3(0, 0, 1.8), ml::ROT3_UNITY);
    sphere->set_radius(1);
    sphere->outer_color = scenery.colors.get("green");

    ml::Frame* pole = scenery.root.add<ml::Frame>();
    pole->set_name_pos_rot("pole", ml::Vec3(0, 0, 0.5), ml::ROT3_UNITY);

    ml::Cylinder* pole1 = pole->add<ml::Cylinder>();
    pole1->set_name_pos_rot("pole1", ml::Vec3(0, 0, 0.0), ml::ROT3_UNITY);
    pole1->set_radius_and_length(0.25, 1.0);
    pole1->outer_color = scenery.colors.get("red");

    ml::Cylinder* fork = pole->add<ml::Cylinder>();
    fork->set_name_pos_rot("fork", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
    fork->set_cylinder(0.1, ml::VEC3_ORIGIN, ml::Vec3(0, 0.5, 0.5));
    fork->outer_color = scenery.colors.get("red");

    ml::Disc* ground = scenery.root.add<ml::Disc>();
    ground->set_name_pos_rot("ground", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
    ground->set_radius(3.0);
    ground->outer_color = scenery.colors.get("grass_green");

    scenery.root.init_tree_based_on_mother_child_relations();
}
