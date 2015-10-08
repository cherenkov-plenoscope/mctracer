#include <iostream> 
#include <string>

#include "gtest/gtest.h"
#include "Sphere.h"
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
  Frame *Mworld = file2world.world();

  // name of default world
  EXPECT_EQ( Mworld->get_name_of_frame() , "world");

  // default world has no children
  EXPECT_EQ( Mworld->get_number_of_children() , 0 );

  // position of default world in mother frame is null vector
  EXPECT_EQ( 
    *Mworld->get_position_in_mother(),
    Vector3D::null
  );

  // rotation of default world in mother frame is null
  EXPECT_EQ( 
    *Mworld->get_rotation_in_mother(),
    Rotation3D::null
  );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadEmptyXML) {

  WorldFactory file2world;
  Frame *Mworld = NULL;

  try{

    file2world.load("./test_scenery/empty.xml");
    Mworld = file2world.world();

  }catch(std::exception& error){
    cout << error.what() << endl;
  } 

  // name of default world
  EXPECT_EQ( Mworld->get_name_of_frame() , "world");
  // no children in world
  EXPECT_EQ( Mworld->get_number_of_children() , 0 );
  // enclosing radius must be zero
  EXPECT_EQ( 0.0, Mworld->get_radius_of_sphere_enclosing_all_children() );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadNotExistingFile) {

  WorldFactory file2world;

  string xml_file = "./not_existing_path/not_existing_file.xml";

  try{

    file2world.load(xml_file);
    Frame *Mworld = file2world.world();

    // the root of a file is always called world also the file does not 
    // exist
    EXPECT_EQ( Mworld->get_name_of_frame() , "world");

  }catch(XmlIoException& error){

    //check whether the exception knows the bad file path or not
    EXPECT_EQ( error.filename() , xml_file );
  }
  catch(std::exception& error){

    // only XmlIoException exceptions are acceptable
    cout << error.what() << endl;
    EXPECT_EQ( true , false );
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadFileWithUnknownObject) {

  WorldFactory file2world;

  string xml_file = 
  "./test_scenery/unkown_object_in_line_17_MarioMuellerWesternhagen.xml";

  try{

    file2world.load(xml_file);
    Frame *Mworld = file2world.world();

    EXPECT_EQ( Mworld->get_name_of_frame() , "world");

  }catch(UnknownItem& unknown){

    //error.ReportException();
    //check whether the exception knows the bad file path or not
    EXPECT_EQ( unknown.name() , "MarioMuellerWesternhagen" );
  }
  catch(std::exception& error){

    //only UnknownObject exceptions are acceptable
    cout << error.what();
    EXPECT_EQ( true , false );
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, IncludeXMLFiles) {
try{

  WorldFactory file2world;

  string xml_file = "./test_scenery/including_other_xml_files.xml";

  file2world.load(xml_file);

  //Frame *Mworld = file2world.world();

  }catch(std::exception& error){
    cout << error.what();
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, MultipleUsageOfName) {
try{

    WorldFactory file2world;
    string xml_file = "./test_scenery/multiple_usage_of_name_klaus.xml";
    file2world.load(xml_file);

  }catch(MultipleUseage& multiple){
    EXPECT_EQ( multiple.name() , "/house/chimney/klaus" );
  }
  catch(std::exception& error){
    cout << error.what();
  }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, first_rot_z_then_new_pointing_for_z) {
  WorldFactory fab;
  fab.load("./test_scenery/rotation_based_on_z_pointing.xml");
  Frame *geometry = fab.world();

  std::cout << geometry->get_tree_print();

  TracerSettings settings;  
  //FreeOrbitCamera free(geometry, &settings);
}