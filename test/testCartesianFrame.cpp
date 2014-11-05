#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../CartesianFrame.h"
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
  file2world.load_file(xml_file);
  CartesianFrame *world = file2world.get_pointer_to_world();

  // Now get the pointer to the frame we are looking for
  // In the case of "tree/pole" we know that it must be in the test xml file.
  const CartesianFrame* SpecificFrameInWorldWeAreLookingFor = 
  world->get_pointer_to_specific_frame( "/tree/pole" );

  EXPECT_EQ("pole", SpecificFrameInWorldWeAreLookingFor->get_name_of_frame());

  // Lets do a second test with one additional layer
  SpecificFrameInWorldWeAreLookingFor = world->
  get_pointer_to_specific_frame( "/house/chimney/chimney_wall_4" );

  if(SpecificFrameInWorldWeAreLookingFor == NULL)
    std::cout << "OMG the Pointer is NULL!!!\n";

  EXPECT_EQ(
    "chimney_wall_4", 
    SpecificFrameInWorldWeAreLookingFor->get_name_of_frame()
  );
  // In addition we test if the path is restored correctly by the frame
  std::string RestoredPath = 
  SpecificFrameInWorldWeAreLookingFor->get_path();

  EXPECT_EQ( "/house/chimney/chimney_wall_4" , RestoredPath );

  // Now lets look something up which is not in our test xml file.
  // In this case a NULL pointer is expected to be returned
  SpecificFrameInWorldWeAreLookingFor = 
  world->get_pointer_to_specific_frame( "your/Mama" );

  EXPECT_EQ( NULL, SpecificFrameInWorldWeAreLookingFor );
}
//----------------------------------------------------------------------