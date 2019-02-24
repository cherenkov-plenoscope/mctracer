// Copyright 2014 Sebastian A. Mueller
#include <array>
#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/HomTra3.h"
#include "scenery/primitive/Primitive.h"
using std::string;
using std::stringstream;
using std::vector;
using namespace relleums;

class FrameTest : public ::testing::Test {};

TEST_F(FrameTest, assert_name_is_valid) {
    Vec3 pos = VEC3_ORIGIN;
    Rot3 rot = ROT3_UNITY;

    Frame Peter;
    EXPECT_NO_THROW(Peter.set_name_pos_rot("A_nice_name", pos, rot));

    EXPECT_THROW(
        Peter.set_name_pos_rot("I feel like using whitespaces", pos, rot),
        std::invalid_argument);

    EXPECT_THROW(
        Peter.set_name_pos_rot("I\tfeel\rlike\tusing\nwhitespaces", pos, rot),
        std::invalid_argument);

    EXPECT_THROW(Peter.set_name_pos_rot("", pos, rot), std::invalid_argument);

    EXPECT_THROW(
        Peter.set_name_pos_rot(
            "I/feel/like/using/the/delimiter/symbol", pos, rot),
        std::invalid_argument);

    EXPECT_THROW(
        Peter.set_name_pos_rot(" ", pos, rot),
        std::invalid_argument);
}

TEST_F(FrameTest, duplicate_name_of_children_frames) {
    Frame Peter;
    Peter.set_name_pos_rot("peter", VEC3_ORIGIN, ROT3_UNITY);

    Frame* Klaus1 = Peter.append<Frame>();
    Klaus1->set_name_pos_rot("klaus", VEC3_ORIGIN, ROT3_UNITY);

    Frame* Klaus2 = Peter.append<Frame>();
    Klaus2->set_name_pos_rot("klaus", VEC3_ORIGIN, ROT3_UNITY);

    EXPECT_THROW(
        Peter.assert_no_children_duplicate_names(),
        std::invalid_argument);
}

TEST_F(FrameTest, set_frame) {
    Vec3 pos(1.3, 3.7, 4.2);
    Rot3 rot(3.1, 4.1, 7.7);

    Frame Peter;
    Peter.set_name_pos_rot("A_nice_name", pos, rot);

    EXPECT_TRUE(pos == Peter.get_position_in_mother());
    EXPECT_TRUE(rot == Peter.get_rotation_in_mother());

    HomTra3 T_frame2mother;
    T_frame2mother.set_transformation(rot, pos);

    EXPECT_EQ(T_frame2mother, *Peter.frame2mother());
}

TEST_F(FrameTest, re_set_frame) {
    Vec3 pos(1.3, 3.7, 4.2);
    Rot3 rot(3.1, 4.1, 7.7);

    Frame peter;
    peter.set_name_pos_rot("a_name", pos, rot);
    Frame* hans = peter.append<Frame>();
    hans->set_name_pos_rot("child_of_peter", Vec3(1.0, 2.0, 3.0), ROT3_UNITY);

    EXPECT_EQ(1u, peter.get_children()->size());
    EXPECT_TRUE(pos == peter.get_position_in_mother());
    EXPECT_TRUE(rot == peter.get_rotation_in_mother());

    peter.set_name_pos_rot(
        "another_name",
        Vec3(1.0, 2.0, 3.0),
        Rot3(0.1, 0.2, 0.3));

    EXPECT_EQ(1u, peter.get_children()->size());
    EXPECT_TRUE(Vec3(1.0, 2.0, 3.0) == peter.get_position_in_mother());
    EXPECT_TRUE(Rot3(0.1, 0.2, 0.3) == peter.get_rotation_in_mother());
}

TEST_F(FrameTest, root_of_world_on_complete_tree) {
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
    EXPECT_EQ(&tree, tree.get_root());
    EXPECT_EQ(&tree, leaf1->get_root());
    EXPECT_EQ(&tree, leaf2->get_root());
    EXPECT_EQ(&tree, branch->get_root());
    EXPECT_EQ(&tree, leaf1_on_branch->get_root());
    EXPECT_EQ(&tree, leaf2_on_branch->get_root());
}

TEST_F(FrameTest, root_frame_default) {
    // A single frame with no relations set (post initialized) is its own root.
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);
    EXPECT_EQ(&tree, tree.get_root());
}

TEST_F(FrameTest, cluster_frames_during_tree_initializing) {
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

    EXPECT_EQ(count, tree.get_children()->size());
    tree.init_tree_based_on_mother_child_relations();

    EXPECT_TRUE(count > tree.get_children()->size());
    EXPECT_TRUE(FRAME_MAX_NUMBER_CHILDREN >= tree.get_children()->size());
}

TEST_F(FrameTest, clustering_frames_which_are_stucked_close_together) {
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

    EXPECT_EQ(number_facets, tree.get_children()->size());
    tree.init_tree_based_on_mother_child_relations();

    // The clustering will not help (reduce the number of children)
    // because all the geometry is stucked on top of each other.
    EXPECT_EQ(number_facets, tree.get_children()->size());
}
#include "Core/RayAndFrame.h"

TEST_F(FrameTest, removing_a_non_existing_cild) {
    Frame tree;
    tree.set_name_pos_rot("tree", VEC3_ORIGIN, ROT3_UNITY);

    Frame another_tree;
    tree.set_name_pos_rot("another_tree", VEC3_ORIGIN, ROT3_UNITY);

    EXPECT_THROW(
        tree.erase(&another_tree),
        std::out_of_range);
}

TEST_F(FrameTest, removing_a_cild) {
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

    EXPECT_EQ(tree.get_bounding_sphere_radius(), 0.0);
    EXPECT_EQ(pole->get_bounding_sphere_radius(), 0.0);

    tree.init_tree_based_on_mother_child_relations();

    EXPECT_GT(tree.get_bounding_sphere_radius(), 0.0);
    EXPECT_GT(pole->get_bounding_sphere_radius(), 0.0);

    Ray ray(Vec3(0, -5, -1.8), Vec3(0, 1, 0));
    Intersection isec = RayAndFrame::first_intersection(&ray, &tree);
    EXPECT_FALSE(isec.does_intersect());

    // Append a temporary frame
    Frame* temp = tree.append<Frame>();
    tree.set_name_pos_rot("temp", VEC3_ORIGIN, ROT3_UNITY);

    Sphere* tball = temp->append<Sphere>();
    tball->set_name_pos_rot("tball", Vec3(0, 0, -1.8), ROT3_UNITY);
    tball->set_radius(1);
    tball->set_outer_color(&COLOR_GREEN);

    tree.init_tree_based_on_mother_child_relations();

    isec = RayAndFrame::first_intersection(&ray, &tree);
    ASSERT_TRUE(isec.does_intersect());
    EXPECT_EQ(isec.get_object(), tball);

    // Erase temporary frame
    tree.erase(temp);
    tree.init_tree_based_on_mother_child_relations();

    isec = RayAndFrame::first_intersection(&ray, &tree);
    EXPECT_FALSE(isec.does_intersect());
}
