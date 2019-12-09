// Copyright 2014 Sebastian A. Mueller
#include <array>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("FrameTest: assert_name_is_valid", "[merlict]") {
    ml::Vec3 pos = ml::VEC3_ORIGIN;
    ml::Rot3 rot = ml::ROT3_UNITY;

    ml::Frame Peter;
    CHECK_NOTHROW(Peter.set_name_pos_rot("A_nice_name", pos, rot));

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I feel like using whitespaces", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I\tfeel\rlike\tusing\nwhitespaces", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I/feel/like/using/the/delimiter/symbol", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot(" ", pos, rot), std::invalid_argument);
}

TEST_CASE("FrameTest: duplicate_name_of_children_frames", "[merlict]") {
    ml::Frame Peter;
    Peter.set_name_pos_rot("peter", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> Klaus1 = Peter.add<ml::Frame>();
    Klaus1->set_name_pos_rot("klaus", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> Klaus2 = Peter.add<ml::Frame>();
    Klaus2->set_name_pos_rot("klaus", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    CHECK_THROWS_AS(Peter.assert_no_children_duplicate_names(), std::invalid_argument);
}

TEST_CASE("FrameTest: set_frame", "[merlict]") {
    ml::Vec3 pos(1.3, 3.7, 4.2);
    ml::Rot3 rot(3.1, 4.1, 7.7);

    ml::Frame Peter;
    Peter.set_name_pos_rot("A_nice_name", pos, rot);

    CHECK(pos == Peter.position_in_mother());
    CHECK(rot == Peter.rotation_in_mother());

    ml::HomTra3 T_frame2mother;
    T_frame2mother.set_transformation(rot, pos);

    CHECK(*Peter.frame2mother() == T_frame2mother);
}

TEST_CASE("FrameTest: re_set_frame", "[merlict]") {
    ml::Vec3 pos(1.3, 3.7, 4.2);
    ml::Rot3 rot(3.1, 4.1, 7.7);

    ml::Frame peter;
    peter.set_name_pos_rot("a_name", pos, rot);
    std::shared_ptr<ml::Frame> hans = peter.add<ml::Frame>();
    hans->set_name_pos_rot(
        "child_of_peter",
        ml::Vec3(1.0, 2.0, 3.0),
        ml::ROT3_UNITY);

    CHECK(peter.get_children()->size() == 1u);
    CHECK(pos == peter.position_in_mother());
    CHECK(rot == peter.rotation_in_mother());

    peter.set_name_pos_rot(
        "another_name",
        ml::Vec3(1.0, 2.0, 3.0),
        ml::Rot3(0.1, 0.2, 0.3));

    CHECK(peter.get_children()->size() == 1u);
    CHECK(ml::Vec3(1.0, 2.0, 3.0) == peter.position_in_mother());
    CHECK(ml::Rot3(0.1, 0.2, 0.3) == peter.rotation_in_mother());
}

TEST_CASE("FrameTest: root_of_world_on_complete_tree", "[merlict]") {
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> leaf1 = tree.add<ml::Frame>();
    leaf1->set_name_pos_rot("leaf1", ml::Vec3(1, 0, 0), ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> leaf2 = tree.add<ml::Frame>();
    leaf2->set_name_pos_rot("leaf2", ml::Vec3(-1, 0, 0), ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> branch = tree.add<ml::Frame>();
    branch->set_name_pos_rot("branch", ml::Vec3(0, 0, 1), ml::ROT3_UNITY);

    std::shared_ptr<ml::Frame> leaf1_on_branch = branch->add<ml::Frame>();
    leaf1_on_branch->set_name_pos_rot(
        "leaf1_on_branch",
        ml::Vec3(1, 0, 0),
        ml::ROT3_UNITY);
    std::shared_ptr<ml::Frame> leaf2_on_branch = branch->add<ml::Frame>();
    leaf2_on_branch->set_name_pos_rot(
        "leaf2_on_branch",
        ml::Vec3(0, 1, 0),
        ml::ROT3_UNITY);

    // -----post initialize
    tree.init_tree_based_on_mother_child_relations();

    // -----test
    CHECK(tree.root() == &tree);
    CHECK(leaf1->root() == &tree);
    CHECK(leaf2->root() == &tree);
    CHECK(branch->root() == &tree);
    CHECK(leaf1_on_branch->root() == &tree);
    CHECK(leaf2_on_branch->root() == &tree);
}

TEST_CASE("FrameTest: root_frame_default", "[merlict]") {
    // A single frame with no relations set (post initialized) is its own root.
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    CHECK(tree.root() == &tree);
}

TEST_CASE("FrameTest: cluster_frames_during_tree_initializing", "[merlict]") {
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    double qube_edge = 10.0;

    unsigned int count = 0;
    for (double x = 0.0; x < qube_edge; x++) {
        for (double y = 0.0; y < qube_edge; y++) {
            for (double z = 0.0; z < qube_edge; z++) {
                ml::Vec3 pos(x, y, z);
                std::stringstream name;
                name << "sub_sphere_" << x << "_" << y << "_" << z;
                ml::Sphere* sphere = tree.add<ml::Sphere>();
                sphere->set_name_pos_rot(name.str(), pos, ml::ROT3_UNITY);
                sphere->set_radius(0.5);
                count++;
            }
        }
    }

    CHECK(tree.get_children()->size() == count);
    tree.init_tree_based_on_mother_child_relations();

    CHECK(count > tree.get_children()->size());
    CHECK(ml::FRAME_MAX_NUMBER_CHILDREN >= tree.get_children()->size());
}

TEST_CASE("FrameTest: clustering_frames_which_are_stucked_close_together", "[merlict]") {
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    const unsigned int num_facets = 100;

    for (unsigned int i = 0; i < num_facets; i++) {
        ml::SphereCapWithRectangularBound* facet =
            tree.add<ml::SphereCapWithRectangularBound>();
        facet->set_name_pos_rot(
            "facet"+std::to_string(i),
            ml::VEC3_ORIGIN,
            ml::ROT3_UNITY);
        facet->set_curvature_radius_and_x_y_width(34.0, 1.0, 1.0);
    }

    CHECK(tree.get_children()->size() == num_facets);
    tree.init_tree_based_on_mother_child_relations();

    // The clustering will not help (reduce the number of children)
    // because all the geometry is stucked on top of each other.
    CHECK(tree.get_children()->size() == num_facets);
}
#include "merlict/RayAndFrame.h"

TEST_CASE("FrameTest: removing_a_non_existing_cild", "[merlict]") {
    ml::Frame tree;
    tree.set_name_pos_rot("tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Frame another_tree;
    tree.set_name_pos_rot("another_tree", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    CHECK_THROWS_AS(tree.erase(&another_tree), std::out_of_range);
}

TEST_CASE("FrameTest: removing_a_cild", "[merlict]") {
    ml::Scenery scenery;
    scenery.colors.add("green", ml::COLOR_GREEN);
    scenery.colors.add("red", ml::COLOR_RED);
    scenery.colors.add("grass_green", ml::COLOR_GRASS_GREEN);
    ml::Sphere* sphere = scenery.root.add<ml::Sphere>();
    sphere->set_name_pos_rot("leaf_ball", ml::Vec3(0, 0, 1.8), ml::ROT3_UNITY);
    sphere->set_radius(1);
    sphere->outer_color = scenery.colors.get("green");

    std::shared_ptr<ml::Frame> pole = scenery.root.add<ml::Frame>();
    pole->set_name_pos_rot("pole", ml::Vec3(0, 0, 0.5), ml::ROT3_UNITY);

    ml::Cylinder* pole1 = pole->add<ml::Cylinder>();
    pole1->set_name_pos_rot("pole1", ml::Vec3(0, 0, 0), ml::ROT3_UNITY);
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

    CHECK(0.0 == scenery.root.get_bounding_sphere_radius());
    CHECK(0.0 == pole->get_bounding_sphere_radius());

    scenery.root.init_tree_based_on_mother_child_relations();

    CHECK(0.0 < scenery.root.get_bounding_sphere_radius());
    CHECK(0.0 < pole->get_bounding_sphere_radius());

    ml::Ray ray(ml::Vec3(0, -5, -1.8), ml::Vec3(0, 1, 0));
    ml::Intersection isec = ml::rays_first_intersection_with_frame(
        &ray,
        &scenery.root);
    CHECK(!isec.does_intersect());

    // Append a temporary frame
    std::shared_ptr<ml::Frame> temp = scenery.root.add<ml::Frame>();
    scenery.root.set_name_pos_rot("temp", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::Sphere* tball = temp->add<ml::Sphere>();
    tball->set_name_pos_rot("tball", ml::Vec3(0, 0, -1.8), ml::ROT3_UNITY);
    tball->set_radius(1);
    tball->outer_color = scenery.colors.get("green");

    scenery.root.init_tree_based_on_mother_child_relations();

    isec = ml::rays_first_intersection_with_frame(&ray, &scenery.root);
    REQUIRE(isec.does_intersect());
    CHECK(tball == isec.object());

    // Erase temporary frame
    scenery.root.erase(temp);
    scenery.root.init_tree_based_on_mother_child_relations();

    isec = ml::rays_first_intersection_with_frame(&ray, &scenery.root);
    CHECK(!isec.does_intersect());
}
