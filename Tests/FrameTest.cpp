#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/HomTra3.h"
#include "Scenery/Primitive/Sphere.h"
#include "Scenery/Primitive/SphereCapWithRectangularBound.h"
#include <array>

using namespace std;

class FrameTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FrameTest, assert_name_is_valid) {

    Vec3 pos = Vec3::null;
    Rot3 rot = Rot3::null;

    Frame Peter;
    EXPECT_NO_THROW(Peter.set_name_pos_rot("A_nice_name",pos,rot));

    EXPECT_THROW(
        Peter.set_name_pos_rot("I feel like using whitespaces",pos,rot),
        Frame::BadName
    );

    EXPECT_THROW(
        Peter.set_name_pos_rot("I\tfeel\rlike\tusing\nwhitespaces",pos,rot),
        Frame::BadName
    );

    EXPECT_THROW(Peter.set_name_pos_rot("",pos,rot), TracerException);

    EXPECT_THROW(   
        Peter.set_name_pos_rot("I/feel/like/using/the/delimiter/symbol",pos,rot),
        Frame::BadName
    );

    EXPECT_THROW(
        Peter.set_name_pos_rot(" ",pos,rot),
        Frame::BadName
    );
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, duplicate_name_of_children_frames) {

    Frame Peter;
    Peter.set_name_pos_rot("peter", Vec3::null, Rot3::null);

    Frame* Klaus1 = Peter.append<Frame>();
    Klaus1->set_name_pos_rot("klaus", Vec3::null, Rot3::null);

    Frame* Klaus2 = Peter.append<Frame>();
    Klaus2->set_name_pos_rot("klaus", Vec3::null, Rot3::null);

    EXPECT_THROW(
        Peter.assert_no_children_duplicate_names(),
        Frame::DuplicateChildName
    );
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, set_frame) {

    Vec3 pos(1.3,3.7,4.2);
    Rot3 rot(3.1,4.1,7.7);

    Frame Peter;
    Peter.set_name_pos_rot("A_nice_name", pos, rot);

    EXPECT_TRUE(pos == Peter.get_position_in_mother());
    EXPECT_TRUE(rot == Peter.get_rotation_in_mother());

    HomTra3 T_frame2mother;
    T_frame2mother.set_transformation(rot, pos);

    EXPECT_EQ(T_frame2mother, *Peter.frame2mother());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, re_set_frame) {

    Vec3 pos(1.3,3.7,4.2);
    Rot3 rot(3.1,4.1,7.7);

    Frame peter;
    peter.set_name_pos_rot("a_name", pos, rot);
    Frame* hans = peter.append<Frame>();
    hans->set_name_pos_rot("child_of_peter", Vec3(1.0, 2.0, 3.0), Rot3::null);

    EXPECT_EQ(1u, peter.get_children()->size());
    EXPECT_TRUE(pos == peter.get_position_in_mother());
    EXPECT_TRUE(rot == peter.get_rotation_in_mother());

    peter.set_name_pos_rot("another_name", Vec3(1.0, 2.0, 3.0), Rot3(0.1, 0.2, 0.3));
    
    EXPECT_EQ(1u, peter.get_children()->size());
    EXPECT_TRUE(Vec3(1.0, 2.0, 3.0) == peter.get_position_in_mother());
    EXPECT_TRUE(Rot3(0.1, 0.2, 0.3) == peter.get_rotation_in_mother());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, root_of_world_on_complete_tree) {

    //-----define frames
    Frame tree;
    tree.set_name_pos_rot("tree" ,Vec3::null, Rot3::null);

    Frame* leaf1 = tree.append<Frame>();
    leaf1->set_name_pos_rot("leaf1" ,Vec3(1.0,0.0,0.0), Rot3::null);

    Frame* leaf2 = tree.append<Frame>();
    leaf2->set_name_pos_rot("leaf2" ,Vec3(-1.0,0.0,0.0), Rot3::null);

    Frame* branch = tree.append<Frame>();
    branch->set_name_pos_rot("branch" ,Vec3(0.0,0.0,1.0), Rot3::null);

    Frame* leaf1_on_branch = branch->append<Frame>();
    leaf1_on_branch->set_name_pos_rot(
        "leaf1_on_branch",
        Vec3(1.0,0.0,0.0),
        Rot3::null
    );
    Frame* leaf2_on_branch = branch->append<Frame>();
    leaf2_on_branch->set_name_pos_rot(
        "leaf2_on_branch",
        Vec3(0.0,1.0,0.0),
        Rot3::null
    );

    //-----post initialize 
    tree.init_tree_based_on_mother_child_relations();

    //-----test
    EXPECT_EQ(&tree, tree.get_root());
    EXPECT_EQ(&tree, leaf1->get_root());
    EXPECT_EQ(&tree, leaf2->get_root());
    EXPECT_EQ(&tree, branch->get_root());
    EXPECT_EQ(&tree, leaf1_on_branch->get_root());
    EXPECT_EQ(&tree, leaf2_on_branch->get_root());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, root_frame_default) {

    //A single frame with no relations set (post initialized) is its own root. 
    Frame tree;
    tree.set_name_pos_rot("tree" ,Vec3::null, Rot3::null);
    EXPECT_EQ(&tree, tree.get_root());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, cluster_frames_during_tree_initializing) {

    Frame tree;
    tree.set_name_pos_rot("tree" ,Vec3::null, Rot3::null);
    double qube_edge = 10.0;

    uint count = 0;
    for(double x=0.0; x<qube_edge; x++) {
        for(double y=0.0; y<qube_edge; y++) {
            for(double z=0.0; z<qube_edge; z++) {
                Vec3 pos(x,y,z);
                stringstream name;
                name << "sub_sphere_" << x << "_" << y << "_" << z;
                Sphere* sphere = tree.append<Sphere>();
                sphere->set_name_pos_rot(name.str(), pos, Rot3::null);
                sphere->set_radius(0.5);
                count++;
            }
        }
    }

    EXPECT_EQ(count, tree.get_children()->size());
    tree.init_tree_based_on_mother_child_relations();

    EXPECT_TRUE(count > tree.get_children()->size());
    EXPECT_TRUE(Frame::max_number_of_children >= tree.get_children()->size());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, clustering_frames_which_are_stucked_close_together) {

    Frame tree;
    tree.set_name_pos_rot("tree" ,Vec3::null, Rot3::null);
    const uint number_facets = 100;

    for(uint i=0; i<number_facets; i++) {
        SphereCapWithRectangularBound* facet = tree.append<SphereCapWithRectangularBound>();
        facet->set_name_pos_rot("facet"+std::to_string(i), Vec3::null, Rot3::null);
        facet->set_curvature_radius_and_x_y_width(34.0, 1.0, 1.0);
    }

    EXPECT_EQ(number_facets, tree.get_children()->size());
    tree.init_tree_based_on_mother_child_relations();

    //The clustering will not help (reduce the number of children) 
    //because all the geometry is stucked on top of each other.
    EXPECT_EQ(number_facets, tree.get_children()->size());
}