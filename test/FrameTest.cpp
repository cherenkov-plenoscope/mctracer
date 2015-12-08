#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/HomoTrafo3D.h"

using namespace std;

class CartesianFrameTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(CartesianFrameTest, assert_name_is_valid) {

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
TEST_F(CartesianFrameTest, set_frame) {

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
TEST_F(CartesianFrameTest, cluster_frames_during_tree_initializing) {

    Frame tree;
    tree.set_name_pos_rot("tree" ,Vector3D::null, Rotation3D::null);

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
    EXPECT_TRUE(Frame::max_number_of_children_in_frame >= tree.get_number_of_children());
}
//------------------------------------------------------------------------------