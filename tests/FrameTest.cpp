// Copyright 2014 Sebastian A. Mueller
#include <array>
#include "catch.hpp"
#include "merlict/merlict.h"
using std::string;
using std::stringstream;
using std::vector;
using namespace relleums;



TEST_CASE("FrameTest: assert_name_is_valid", "[mctracer]") {
    Vec3 pos = VEC3_ORIGIN;
    Rot3 rot = ROT3_UNITY;

    Frame Peter;
    CHECK_NOTHROW(Peter.set_name_pos_rot("A_nice_name", pos, rot));

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I feel like using whitespaces", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I\tfeel\rlike\tusing\nwhitespaces", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot("I/feel/like/using/the/delimiter/symbol", pos, rot), std::invalid_argument);

    CHECK_THROWS_AS(Peter.set_name_pos_rot(" ", pos, rot), std::invalid_argument);
}

TEST_CASE("FrameTest: duplicate_name_of_children_frames", "[mctracer]") {
    Frame Peter;
    Peter.set_name_pos_rot("peter", VEC3_ORIGIN, ROT3_UNITY);

    Frame* Klaus1 = Peter.append<Frame>();
    Klaus1->set_name_pos_rot("klaus", VEC3_ORIGIN, ROT3_UNITY);

    Frame* Klaus2 = Peter.append<Frame>();
    Klaus2->set_name_pos_rot("klaus", VEC3_ORIGIN, ROT3_UNITY);

    CHECK_THROWS_AS(Peter.assert_no_children_duplicate_names(), std::invalid_argument);
}

TEST_CASE("FrameTest: set_frame", "[mctracer]") {
    Vec3 pos(1.3, 3.7, 4.2);
    Rot3 rot(3.1, 4.1, 7.7);

    Frame Peter;
    Peter.set_name_pos_rot("A_nice_name", pos, rot);

    CHECK(pos == Peter.get_position_in_mother());
    CHECK(rot == Peter.get_rotation_in_mother());

    HomTra3 T_frame2mother;
    T_frame2mother.set_transformation(rot, pos);

    CHECK(*Peter.frame2mother() == T_frame2mother);
}

TEST_CASE("FrameTest: re_set_frame", "[mctracer]") {
    Vec3 pos(1.3, 3.7, 4.2);
    Rot3 rot(3.1, 4.1, 7.7);

    Frame peter;
    peter.set_name_pos_rot("a_name", pos, rot);
    Frame* hans = peter.append<Frame>();
    hans->set_name_pos_rot("child_of_peter", Vec3(1.0, 2.0, 3.0), ROT3_UNITY);

    CHECK(peter.get_children()->size() == 1u);
    CHECK(pos == peter.get_position_in_mother());
    CHECK(rot == peter.get_rotation_in_mother());

    peter.set_name_pos_rot(
        "another_name",
        Vec3(1.0, 2.0, 3.0),
        Rot3(0.1, 0.2, 0.3));

    CHECK(peter.get_children()->size() == 1u);
    CHECK(Vec3(1.0, 2.0, 3.0) == peter.get_position_in_mother());
    CHECK(Rot3(0.1, 0.2, 0.3) == peter.get_rotation_in_mother());
}

TEST_CASE("FrameTest: root_of_world_on_complete_tree", "[mctracer]") {
    // -----define frames
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);

    Frame* leaf1 = tree.append<Frame>();
    leaf1->set_name_pos_rot("leaf1", Vec3(1.0, 0.0, 0.0), ROT3_UNITY);

    Frame* leaf2 = tree.append<Frame>();
    leaf2->set_name_pos_rot("leaf2", Vec3(-1.0, 0.0, 0.0), ROT3_UNITY);

    Frame* branch = tree.append<Frame>();
    branch->set_name_pos_rot("branch", Vec3(0.0, 0.0, 1.0), ROT3_UNITY);

    Frame* leaf1_on_branch = branch->append<Frame>();
    leaf1_on_branch->set_name_pos_rot(
        "leaf1_on_branch",
        Vec3(1.0, 0.0, 0.0),
        ROT3_UNITY);
    Frame* leaf2_on_branch = branch->append<Frame>();
    leaf2_on_branch->set_name_pos_rot(
        "leaf2_on_branch",
        Vec3(0.0, 1.0, 0.0),
        ROT3_UNITY);

    // -----post initialize
    tree.init_tree_based_on_mother_child_relations();

    // -----test
    CHECK(tree.get_root() == &tree);
    CHECK(leaf1->get_root() == &tree);
    CHECK(leaf2->get_root() == &tree);
    CHECK(branch->get_root() == &tree);
    CHECK(leaf1_on_branch->get_root() == &tree);
    CHECK(leaf2_on_branch->get_root() == &tree);
}

TEST_CASE("FrameTest: root_frame_default", "[mctracer]") {
    // A single frame with no relations set (post initialized) is its own root.
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    CHECK(tree.get_root() == &tree);
}

TEST_CASE("FrameTest: cluster_frames_during_tree_initializing", "[mctracer]") {
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    double qube_edge = 10.0;

    unsigned int count = 0;
    for (double x = 0.0; x < qube_edge; x++) {
        for (double y = 0.0; y < qube_edge; y++) {
            for (double z = 0.0; z < qube_edge; z++) {
                Vec3 pos(x, y, z);
                stringstream name;
                name << "sub_sphere_" << x << "_" << y << "_" << z;
                Sphere* sphere = tree.append<Sphere>();
                sphere->set_name_pos_rot(name.str(), pos, ROT3_UNITY);
                sphere->set_radius(0.5);
                count++;
            }
        }
    }

    CHECK(tree.get_children()->size() == count);
    tree.init_tree_based_on_mother_child_relations();

    CHECK(count > tree.get_children()->size());
    CHECK(FRAME_MAX_NUMBER_CHILDREN >= tree.get_children()->size());
}

TEST_CASE("FrameTest: clustering_frames_which_are_stucked_close_together", "[mctracer]") {
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    const unsigned int number_facets = 100;

    for (unsigned int i = 0; i < number_facets; i++) {
        SphereCapWithRectangularBound* facet =
            tree.append<SphereCapWithRectangularBound>();
        facet->set_name_pos_rot(
            "facet"+std::to_string(i),
            VEC3_ORIGIN,
            ROT3_UNITY);
        facet->set_curvature_radius_and_x_y_width(34.0, 1.0, 1.0);
    }

    CHECK(tree.get_children()->size() == number_facets);
    tree.init_tree_based_on_mother_child_relations();

    // The clustering will not help (reduce the number of children)
    // because all the geometry is stucked on top of each other.
    CHECK(tree.get_children()->size() == number_facets);
}
#include "merlict/RayAndFrame.h"

TEST_CASE("FrameTest: removing_a_non_existing_cild", "[mctracer]") {
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);

    Frame another_tree;
    tree.set_name_pos_rot("another_tree", VEC3_ORIGIN, ROT3_UNITY);

    CHECK_THROWS_AS(tree.erase(&another_tree), std::out_of_range);
}

TEST_CASE("FrameTest: removing_a_cild", "[mctracer]") {
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);

    Sphere* sphere = tree.append<Sphere>();
    sphere->set_name_pos_rot("leaf_ball", Vec3(0, 0, 1.8), ROT3_UNITY);
    sphere->set_radius(1);
    sphere->set_outer_color(&COLOR_GREEN);

    Frame* pole = tree.append<Frame>();
    pole->set_name_pos_rot("pole", Vec3(0, 0, 0.5), ROT3_UNITY);

    Cylinder* pole1 = pole->append<Cylinder>();
    pole1->set_name_pos_rot("pole1", Vec3(0, 0, 0.0), ROT3_UNITY);
    pole1->set_radius_and_length(0.25, 1.0);
    pole1->set_outer_color(&COLOR_RED);

    Cylinder* fork = pole->append<Cylinder>();
    fork->set_name_pos_rot("fork", Vec3(0, 0, 0), ROT3_UNITY);
    fork->set_cylinder(0.1, VEC3_ORIGIN, Vec3(0, 0.5, 0.5));
    fork->set_outer_color(&COLOR_RED);

    Disc* ground = tree.append<Disc>();
    ground->set_name_pos_rot("ground", Vec3(0, 0, 0), ROT3_UNITY);
    ground->set_radius(3.0);
    ground->set_outer_color(&COLOR_GRASS_GREEN);

    CHECK(0.0 == tree.get_bounding_sphere_radius());
    CHECK(0.0 == pole->get_bounding_sphere_radius());

    tree.init_tree_based_on_mother_child_relations();

    CHECK(0.0 < tree.get_bounding_sphere_radius());
    CHECK(0.0 < pole->get_bounding_sphere_radius());

    Ray ray(Vec3(0, -5, -1.8), Vec3(0, 1, 0));
    Intersection isec = RayAndFrame::first_intersection(&ray, &tree);
    CHECK(!isec.does_intersect());

    // Append a temporary frame
    Frame* temp = tree.append<Frame>();
    tree.set_name_pos_rot("temp", VEC3_ORIGIN, ROT3_UNITY);

    Sphere* tball = temp->append<Sphere>();
    tball->set_name_pos_rot("tball", Vec3(0, 0, -1.8), ROT3_UNITY);
    tball->set_radius(1);
    tball->set_outer_color(&COLOR_GREEN);

    tree.init_tree_based_on_mother_child_relations();

    isec = RayAndFrame::first_intersection(&ray, &tree);
    REQUIRE(isec.does_intersect());
    CHECK(tball == isec.get_object());

    // Erase temporary frame
    tree.erase(temp);
    tree.init_tree_based_on_mother_child_relations();

    isec = RayAndFrame::first_intersection(&ray, &tree);
    CHECK(!isec.does_intersect());
}
