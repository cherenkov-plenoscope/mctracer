// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "scenery/primitive/Primitive.h"
#include "Visual/FlyingCamera.h"
using std::string;
using std::vector;
using namespace relleums;

class OwnerShipTest : public ::testing::Test {};

TEST_F(OwnerShipTest, own) {
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

    tree.init_tree_based_on_mother_child_relations();

    // PropagationConfig set;
    // FlyingCamera fly(&tree, &set);
}
