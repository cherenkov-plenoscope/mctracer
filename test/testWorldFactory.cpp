#include <iostream> 
#include <string>

#include "gtest/gtest.h"
#include "../WorldFactory.h"

using namespace std;

// The fixture for testing class Foo.
class WorldFactoryTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  WorldFactoryTest() {
    // You can do set-up work for each test here.
  }

  virtual ~WorldFactoryTest() {
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
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, DefaultWorld) {

  WorldFactory file2world;
  CartesianFrame *Mworld = file2world.get_pointer_to_world();

  // name of default world
  EXPECT_EQ( *Mworld->get_pointer_to_name_of_frame() , "world");

  // default world has no children
  EXPECT_EQ( Mworld->get_number_of_children() , 0 );

  // position of default world in mother frame is null vector
  Vector3D null_vector;
  null_vector.set_null_vector();

  EXPECT_EQ( 
    *Mworld->get_pointer_to_position_of_frame_in_mother_frame(),
    null_vector
  );

  // rotation of default world in mother frame is null
  Rotation3D null_rotation;
  null_rotation.set_null_rotation();

  EXPECT_EQ( 
    *Mworld->get_pointer_to_rotation_of_frame_in_mother_frame(),
    null_rotation
  );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadEmptyXML) {

  WorldFactory file2world;
  CartesianFrame *Mworld = NULL;

  try{

    file2world.load_file("./test_scenery/empty.xml");
    Mworld = file2world.get_pointer_to_world();

  }catch(TracerException& error){

    error.ReportException();
  } 

  // name of default world
  EXPECT_EQ( *Mworld->get_pointer_to_name_of_frame() , "world");
  // no children in world
  EXPECT_EQ( Mworld->get_number_of_children() , 0 );
  // enclosing radius must be zero
  EXPECT_EQ( 
    *Mworld->get_pointer_to_radius_of_sphere_enclosing_all_children(),
     0.0
  );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadNotExistingFile) {

  WorldFactory file2world;

  string xml_file = "./not_existing_path/not_existing_file.xml";

  try{

    file2world.load_file(xml_file);
    CartesianFrame *Mworld = file2world.get_pointer_to_world();

    // the root of a file is always called world also the file does not 
    // exist
    EXPECT_EQ( *Mworld->get_pointer_to_name_of_frame() , "world");

  }catch(BadXMLFile& error){

    //error.ReportException();
    //check whether the exception knows the bad file path or not
    EXPECT_EQ( error.FileName() , xml_file );
  }
  catch(TracerException& error){

    // only BadXMLFile exceptions are acceptable
    error.ReportException();
    EXPECT_EQ( true , false );
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadFileWithUnknownObject) {

  WorldFactory file2world;

  string xml_file = 
  "./test_scenery/unkown_object_in_line_17_MarioMuellerWesternhagen.xml";

  try{

    file2world.load_file(xml_file);
    CartesianFrame *Mworld = file2world.get_pointer_to_world();

    // the root of a file is always called world also the file does not 
    // exist
    EXPECT_EQ( *Mworld->get_pointer_to_name_of_frame() , "world");

  }catch(UnknownObject& error){

    //error.ReportException();
    //check whether the exception knows the bad file path or not
    EXPECT_EQ( error.GetUnknownObjectName() , "MarioMuellerWesternhagen" );
  }
  catch(TracerException& error){

    //only UnknownObject exceptions are acceptable
    error.ReportException();
    EXPECT_EQ( true , false );
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, IncludeXMLFiles) {
try{

  WorldFactory file2world;

  string xml_file = "./test_scenery/including_other_xml_files.xml";

  file2world.load_file(xml_file);

  CartesianFrame *Mworld = file2world.get_pointer_to_world();

  }catch(TracerException& error){
    error.ReportException();
  }
}