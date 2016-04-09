#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/HomoTrafo3D.h"
#include "Geometry/Sphere.h"

using namespace std;

class FrameTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FrameTest, assert_name_is_valid) {

  Vector3D    pos = Vector3D::null;
  Rotation3D  rot = Rotation3D::null;

  Frame Peter;
//---------------------------------
  EXPECT_NO_THROW( Peter.set_name_pos_rot("A_nice_name",pos,rot));
//---------------------------------
  EXPECT_THROW(
    Peter.set_name_pos_rot("I feel like using whitespaces",pos,rot),
    TracerException
  );
//---------------------------------
  EXPECT_THROW(
    Peter.set_name_pos_rot("I\tfeel\rlike\tusing\nwhitespaces",pos,rot),
    TracerException
  );
//---------------------------------
  EXPECT_THROW(Peter.set_name_pos_rot("",pos,rot), TracerException);
//---------------------------------
  EXPECT_THROW(   
    Peter.set_name_pos_rot("I/feel/like/using/the/delimiter/symbol",pos,rot),
    TracerException
  );
//---------------------------------
  EXPECT_THROW(
    Peter.set_name_pos_rot(" ",pos,rot),
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, duplicate_name_of_children_frames) {

  Frame Peter("peter", Vector3D::null, Rotation3D::null);
  Frame Klaus1("klaus", Vector3D::null, Rotation3D::null);
  Frame Klaus2("klaus", Vector3D::null, Rotation3D::null);

  Peter.set_mother_and_child(&Klaus1);
  Peter.set_mother_and_child(&Klaus2);

  EXPECT_THROW(
    Peter.assert_no_children_duplicate_names()
    ,
    Frame::DuplicateChildName
  );
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, set_frame) {

  Vector3D    pos(1.3,3.7,4.2);
  Rotation3D  rot(3.1,4.1,7.7);

  Frame Peter;
  Peter.set_name_pos_rot("A_nice_name",pos,rot);

  EXPECT_FALSE(Peter.has_mother()) << "There must not be a mother after set";
  EXPECT_FALSE(Peter.has_children()) << "There must not be children after set";
  EXPECT_FALSE(Peter.has_child_with_name("any_child"));

  EXPECT_EQ(pos, *Peter.get_position_in_mother());
  EXPECT_EQ(rot, *Peter.get_rotation_in_mother());

  HomoTrafo3D T_frame2mother;
  T_frame2mother.set_transformation(rot, pos);

  EXPECT_EQ(T_frame2mother, *Peter.frame2mother());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, root_of_world_on_complete_tree) {

  //-----define frames
  Frame tree;
  tree.set_name_pos_rot("tree" ,Vector3D::null, Rotation3D::null);

  Frame leaf1;
  leaf1.set_name_pos_rot("leaf1" ,Vector3D(1.0,0.0,0.0), Rotation3D::null);

  Frame leaf2;
  leaf2.set_name_pos_rot("leaf2" ,Vector3D(-1.0,0.0,0.0), Rotation3D::null);

  Frame branch;
  branch.set_name_pos_rot("branch" ,Vector3D(0.0,0.0,1.0), Rotation3D::null);

  Frame leaf1_on_branch;
  leaf1_on_branch.set_name_pos_rot(
    "leaf1_on_branch",
    Vector3D(1.0,0.0,0.0),
    Rotation3D::null
  );

  Frame leaf2_on_branch;
  leaf2_on_branch.set_name_pos_rot(
    "leaf2_on_branch",
    Vector3D(0.0,1.0,0.0),
    Rotation3D::null
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
TEST_F(FrameTest, root_of_world_default) {

  Frame tree;
  tree.set_name_pos_rot("tree" ,Vector3D::null, Rotation3D::null);
  EXPECT_EQ(&tree, tree.get_root_of_world());
}
//------------------------------------------------------------------------------
TEST_F(FrameTest, cluster_frames_during_tree_initializing) {

    Frame tree;
    tree.set_name_pos_rot("tree" ,Vector3D::null, Rotation3D::null);
    EXPECT_EQ(&tree, tree.get_root_of_world());

    double qube_edge = 10.0;

    uint count = 0;
    for(double x=0.0; x<qube_edge; x++) {
        for(double y=0.0; y<qube_edge; y++) {
            for(double z=0.0; z<qube_edge; z++) {
                count++;

                Vector3D pos(x,y,z);

                std::stringstream name;
                name << "sub_sphere_" << x << "_" << y << "_" << z;

                Sphere* ball = new Sphere(name.str(), pos, Rotation3D::null);
                ball->set_sphere_radius(0.5);

                tree.set_mother_and_child(ball);
            }
        }
    }

    EXPECT_EQ(count, tree.get_number_of_children());

    tree.init_tree_based_on_mother_child_relations();

    EXPECT_TRUE(count > tree.get_number_of_children());
    EXPECT_TRUE(Frame::max_number_of_children >= tree.get_number_of_children());
}
//------------------------------------------------------------------------------