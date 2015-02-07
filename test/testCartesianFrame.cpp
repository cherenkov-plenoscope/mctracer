#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../CartesianFrame.h"
#include "../Vector3D.h"
#include "../Rotation3D.h"
#include "../HomoTrafo3D.h"
//#include "../Photon.h"

using namespace std;

// The fixture for testing class Foo.
class CartesianFrameTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  CartesianFrameTest() {
    // You can do set-up work for each test here.
  }

  virtual ~CartesianFrameTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(CartesianFrameTest, find_specific_frame) {
  
  // load some world
  string xml_file = "./test_scenery/including_other_xml_files.xml";
  WorldFactory file2world;
  file2world.load(xml_file);
  CartesianFrame *world = file2world.world();

  // Now get the pointer to the frame we are looking for
  // In the case of "tree/pole" we know that it must be in the test xml file.
  const CartesianFrame* SpecificFrameInWorldWeAreLookingFor = 
  world->get_frame_in_tree_by_path( "/tree/pole" );

  EXPECT_EQ("pole", SpecificFrameInWorldWeAreLookingFor->get_name_of_frame());

  // Lets do a second test with one additional layer
  SpecificFrameInWorldWeAreLookingFor = world->
  get_frame_in_tree_by_path( "/house/chimney/chimney_wall_4" );

  if(SpecificFrameInWorldWeAreLookingFor == NULL)
    std::cout << "OMG the Pointer is NULL!!!\n";

  EXPECT_EQ(
    "chimney_wall_4", 
    SpecificFrameInWorldWeAreLookingFor->get_name_of_frame()
  );
  // In addition we test if the path is restored correctly by the frame
  std::string RestoredPath = 
  SpecificFrameInWorldWeAreLookingFor->get_path_in_tree_of_frames();

  EXPECT_EQ( "/house/chimney/chimney_wall_4" , RestoredPath );

  // Now lets look something up which is not in our test xml file.
  // In this case a NULL pointer is expected to be returned
  SpecificFrameInWorldWeAreLookingFor = 
  world->get_frame_in_tree_by_path( "your/Mama" );

  EXPECT_EQ( NULL, SpecificFrameInWorldWeAreLookingFor );
}
//----------------------------------------------------------------------
TEST_F(CartesianFrameTest, assert_name_is_valid) {

  Vector3D    pos(0.0,0.0,0.0);
  Rotation3D  rot(0.0,0.0,0.0);

  CartesianFrame Peter;
//---------------------------------
  bool error_detected = false;
  try{
    Peter.set_frame("A_nice_name",pos,rot);
  }catch(std::exception &error){
    cout << error.what();
    error_detected = true;
  }
  EXPECT_FALSE(error_detected);
//---------------------------------
  error_detected = false;
  try{
    Peter.set_frame("I feel like using whitespaces",pos,rot);
  }catch(TracerException &error){
    EXPECT_EQ(WHITE_SPACE_IN_NAME_OF_FRAME, error.type());
    error_detected = true;
  }
  EXPECT_TRUE(error_detected);
//---------------------------------
  error_detected = false;
  try{
    Peter.set_frame("I\tfeel\rlike\tusing\nwhitespaces",pos,rot);
  }catch(TracerException &error){
    EXPECT_EQ(WHITE_SPACE_IN_NAME_OF_FRAME, error.type());
    error_detected = true;
  }
  EXPECT_TRUE(error_detected);
//---------------------------------
  error_detected = false;
  try{
    Peter.set_frame("",pos,rot);
  }catch(TracerException &error){
    EXPECT_EQ(EMPTY_NAME_OF_FRAME, error.type());
    error_detected = true;
  }
  EXPECT_TRUE(error_detected);  
//---------------------------------
  error_detected = false;
  try{
    Peter.set_frame("I/feel/like/using/the/delimiter/symbol",pos,rot);
  }catch(TracerException &error){
    EXPECT_EQ(DELIMITER_SYMBOL_IN_NAME_OF_FRAME, error.type());
    error_detected = true;
  }
  EXPECT_TRUE(error_detected); 
//---------------------------------
  error_detected = false;
  try{
    Peter.set_frame(" ",pos,rot);
  }catch(TracerException &error){
    EXPECT_EQ(WHITE_SPACE_IN_NAME_OF_FRAME, error.type());
    error_detected = true;
  }
  EXPECT_TRUE(error_detected); 
}
//==============================================================================
TEST_F(CartesianFrameTest, set_frame) {

  Vector3D    pos(1.3,3.7,4.2);
  Rotation3D  rot(3.1,4.1,7.7);

  CartesianFrame Peter;
  Peter.set_frame("A_nice_name",pos,rot);

  EXPECT_FALSE(Peter.has_mother()) << "There must not be a mother after set";
  EXPECT_FALSE(Peter.has_children()) << "There must not be children after set";
  EXPECT_FALSE(Peter.has_child_with_name("any_child"));

  EXPECT_EQ(pos, *Peter.get_position_of_frame_in_mother_frame());
  EXPECT_EQ(rot, *Peter.get_rotation_of_frame_in_mother_frame());

  HomoTrafo3D T_frame2mother;
  T_frame2mother.set_transformation(rot, pos);

  EXPECT_EQ(T_frame2mother, *Peter.frame2mother());
}
//==============================================================================
TEST_F(CartesianFrameTest, root_of_world_on_complete_tree) {

  //-----define frames
  CartesianFrame tree;
  tree.set_frame("tree" ,Vector3D(0.0,0.0,0.0), Rotation3D(0.0, 0.0, 0.0));

  CartesianFrame leaf1;
  leaf1.set_frame("leaf1" ,Vector3D(1.0,0.0,0.0), Rotation3D(0.0, 0.0, 0.0));

  CartesianFrame leaf2;
  leaf2.set_frame("leaf2" ,Vector3D(-1.0,0.0,0.0), Rotation3D(0.0, 0.0, 0.0));

  CartesianFrame branch;
  branch.set_frame("branch" ,Vector3D(0.0,0.0,1.0), Rotation3D(0.0, 0.0, 0.0));

  CartesianFrame leaf1_on_branch;
  leaf1_on_branch.set_frame(
    "leaf1_on_branch",
    Vector3D(1.0,0.0,0.0),
    Rotation3D(0.0, 0.0, 0.0)
  );

  CartesianFrame leaf2_on_branch;
  leaf2_on_branch.set_frame(
    "leaf2_on_branch",
    Vector3D(0.0,1.0,0.0),
    Rotation3D(0.0, 0.0, 0.0)
  );

  //-----declare relationschips
  branch.set_mother_and_child(&leaf1_on_branch);
  branch.set_mother_and_child(&leaf2_on_branch);

  tree.set_mother_and_child(&leaf1);
  tree.set_mother_and_child(&leaf2);

  tree.set_mother_and_child(&branch);

  //-----post initialize 
  tree.setup_tree_based_on_mother_child_relations();

  //-----test
  EXPECT_EQ(&tree, tree.get_root_of_world());
  EXPECT_EQ(&tree, leaf1.get_root_of_world());
  EXPECT_EQ(&tree, leaf2.get_root_of_world());
  EXPECT_EQ(&tree, branch.get_root_of_world());
  EXPECT_EQ(&tree, leaf1_on_branch.get_root_of_world());
  EXPECT_EQ(&tree, leaf2_on_branch.get_root_of_world());
}
//==============================================================================
TEST_F(CartesianFrameTest, root_of_world_default) {

  CartesianFrame tree;
  tree.set_frame("tree" ,Vector3D(0.0,0.0,0.0), Rotation3D(0.0, 0.0, 0.0));
  EXPECT_EQ(&tree, tree.get_root_of_world());
}
//==============================================================================