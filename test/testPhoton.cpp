#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Photon.h"
#include "../SurfaceEntity.h"
#include "../Plane.h"
#include "../FreeOrbitCamera.h"
//#include "../ListOfInteractions.h"

using namespace std;

// The fixture for testing class Foo.
class PhotonTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  PhotonTest() {
    // You can do set-up work for each test here.
  }

  virtual ~PhotonTest() {
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
TEST_F(PhotonTest, ConstructorAndGetter) {

  Vector3D support(0.0,0.0,0.0);
  Vector3D direction(0.0,0.0,1.0);
  double wavelength = 433e-9;

  Photon P(support,direction,wavelength);

  EXPECT_EQ(wavelength , P.get_wavelength() );
  EXPECT_EQ(
    "support: (0|0|0)[m], direction: (0|0|1)[m], wavelength: 433[nm]",
    P.get_string() 
  );

  Photon D(0,0,0,0,0,1,wavelength);
  EXPECT_EQ(wavelength , D.get_wavelength() );
  EXPECT_EQ(
    "support: (0|0|0)[m], direction: (0|0|1)[m], wavelength: 433[nm]",
    D.get_string() 
  );

  Ray prototype;
  prototype.set_base(support);
  prototype.set_dir(direction);


  Photon G(prototype,wavelength);
  EXPECT_EQ(wavelength , G.get_wavelength() );
  EXPECT_EQ(
    "support: (0|0|0)[m], direction: (0|0|1)[m], wavelength: 433[nm]",
    G.get_string() 
  );
}
//----------------------------------------------------------------------
TEST_F(PhotonTest, propagation){

  GlobalSettings setup;

  int number_of_bounces = 42;
  setup.set_max_number_of_reflections(number_of_bounces);

  Vector3D    pos(0,0,0);
  Rotation3D  rot(0,0,0);

  // create a test setup with two mirrors bouncing the photon
  CartesianFrame world("world",pos,rot);

  CartesianFrame optical_table("optical table",pos,rot);
  
  ReflectionProperties  refl; 
  ColourProperties      colo;
  pos.set(0.0,0.0,0.0);
  rot.set(0.0,0.0,0.0);
  
  refl.set_reflection_coefficient(0.7);
  colo.set_colour_0to255(200,128,128);

  //------------mirror 1----------------
  Plane mirror1;
  mirror1.set_frame("mirror 1",pos,rot);
  mirror1.set_surface_properties(&refl,&colo);
  mirror1.set_plane(-.5,.5,-.5,.5);
  
  //------------mirror 2----------------
  pos.set(0.0,0.0,1.0);
  rot.set(0.0,0.0,0.0);
  Plane mirror2;
  mirror2.set_frame("mirror 2",pos,rot);
  mirror2.set_surface_properties(&refl,&colo);
  mirror2.set_plane(-.5,.5,-.5,.5);

  //----------declare relationships------------
  optical_table.set_mother_and_child(&mirror1);
  optical_table.set_mother_and_child(&mirror2);

  world.set_mother_and_child(&optical_table);

  //---post initialize the world to calculate all bounding spheres---
  world.post_initialize_me_and_all_my_children();

  //----------free orbit-----------------------
  /*
  FreeOrbitCamera free;
  free.set_free_orbit(&world,&setup);
  free.start_free_orbit();
  */
  //-----------send Photon----------------------
  // the photon is starting in between the to mirrors
  // traveling to the upper mirror
  Vector3D support(0.0,0.0,0.5);
  Vector3D direction(0.0,0.0,1.0);
  double wavelength = 433e-9;
  
  for(int i=0; i<1e2; i++)
  {

    ListOfInteractions history;
    Photon P(support,direction,wavelength);

    P.propagate(&world,&history,&setup);

    //history.show();
    EXPECT_EQ(number_of_bounces*1.0-0.5, history.get_accumulative_distance() );
    EXPECT_EQ(number_of_bounces,history.Interactions.size());
  }
  
}
