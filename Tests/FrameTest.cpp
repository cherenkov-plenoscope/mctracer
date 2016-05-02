#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/HomTra3.h"
#include "Geometry/Sphere.h"

using namespace std;

class FrameTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FrameTest, assert_name_is_valid) {

    Vec3    pos = Vec3::null;
    Rot3  rot = Rot3::null;

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

    Frame Peter("peter", Vec3::null, Rot3::null);
    Frame Klaus1("klaus", Vec3::null, Rot3::null);
    Frame Klaus2("klaus", Vec3::null, Rot3::null);

    Peter.set_mother_and_child(&Klaus1);
    Peter.set_mother_and_child(&Klaus2);

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

    Frame peter("a_name", pos, rot);
    Frame hans("child_of_peter", Vec3(1.0, 2.0, 3.0), Rot3::null);
    peter.set_mother_and_child(&hans);

    EXPECT_EQ(1, peter.get_number_of_children());
    EXPECT_TRUE(pos == peter.get_position_in_mother());
    EXPECT_TRUE(rot == peter.get_rotation_in_mother());

    peter.set_name_pos_rot("another_name", Vec3(1.0, 2.0, 3.0), Rot3(0.1, 0.2, 0.3));
    
    EXPECT_EQ(1, peter.get_number_of_children());
    EXPECT_TRUE(Vec3(1.0, 2.0, 3.0) == peter.get_position_in_mother());
    EXPECT_TRUE(Rot3(0.1, 0.2, 0.3) == peter.get_rotation_in_mother());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, root_of_world_on_complete_tree) {

    //-----define frames
    Frame tree("tree" ,Vec3::null, Rot3::null);
    Frame leaf1("leaf1" ,Vec3(1.0,0.0,0.0), Rot3::null);
    Frame leaf2("leaf2" ,Vec3(-1.0,0.0,0.0), Rot3::null);
    Frame branch("branch" ,Vec3(0.0,0.0,1.0), Rot3::null);
    Frame leaf1_on_branch(
        "leaf1_on_branch",
        Vec3(1.0,0.0,0.0),
        Rot3::null
    );
    Frame leaf2_on_branch(
        "leaf2_on_branch",
        Vec3(0.0,1.0,0.0),
        Rot3::null
    );

    //-----declare relationschips
    branch.set_mother_and_child(&leaf1_on_branch);
    branch.set_mother_and_child(&leaf2_on_branch);
    tree.set_mother_and_child(&leaf1);
    tree.set_mother_and_child(&leaf2);
    tree.set_mother_and_child(&branch);

    //-----post initialize 
    tree.init_tree_based_on_mother_child_relations();

    //-----test
    EXPECT_EQ(&tree, tree.get_root_of_world());
    EXPECT_EQ(&tree, leaf1.get_root_of_world());
    EXPECT_EQ(&tree, leaf2.get_root_of_world());
    EXPECT_EQ(&tree, branch.get_root_of_world());
    EXPECT_EQ(&tree, leaf1_on_branch.get_root_of_world());
    EXPECT_EQ(&tree, leaf2_on_branch.get_root_of_world());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, root_frame_default) {

    //A single frame with no relations set (post initialized) is its own root. 
    Frame tree("tree" ,Vec3::null, Rot3::null);
    EXPECT_EQ(&tree, tree.get_root_of_world());
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
                Sphere* sphere = new Sphere(name.str(), pos, Rot3::null);
                sphere->set_radius(0.5);
                tree.set_mother_and_child(sphere);
                count++;
            }
        }
    }

    EXPECT_EQ(count, tree.get_number_of_children());
    tree.init_tree_based_on_mother_child_relations();

    EXPECT_TRUE(count > tree.get_number_of_children());
    EXPECT_TRUE(Frame::max_number_of_children >= tree.get_number_of_children());
}
//------------------------------------------------------------------------------