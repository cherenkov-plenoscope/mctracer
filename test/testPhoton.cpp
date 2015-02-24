#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Photon.h"
#include "../SurfaceEntity.h"
#include "../Plane.h"
#include "Cameras/FreeOrbitCamera.h"
#include "../PseudoRandomNumberGenerator.h"
#include "../ListOfPropagations.h"

using namespace std;

// The fixture for testing class Foo.
class PhotonTest : public ::testing::Test {
 protected:

  Vector3D origin;
  Vector3D ez;
  double wavelength;

  PhotonTest() {
    origin.set(0.0, 0.0, 0.0);
    ez.set(0.0, 0.0, 1.0);
    wavelength = 433.0e-9;
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
//------------------------------------------------------------------------------
TEST_F(PhotonTest, creation_constructor) {
  Photon pho(origin, ez*1.337, wavelength);

  EXPECT_EQ(ez, pho.Direction());
  EXPECT_EQ(origin, pho.Support());
  EXPECT_EQ(1.0, pho.Direction().norm2());
  EXPECT_EQ(wavelength, pho.get_wavelength());
  ASSERT_FALSE(nullptr == pho.get_propagation_history() );
  EXPECT_EQ(0, pho.get_propagation_history()->size() );
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, PropagationSimpleGeometry){

  GlobalSettings setup;

  int number_of_bounces = 42;
  setup.set_max_number_of_reflections(number_of_bounces);

  Vector3D    pos(0,0,0);
  Rotation3D  rot(0,0,0);

  // create a test setup with two mirrors bouncing the photon
  CartesianFrame world("world",pos,rot);

  CartesianFrame optical_table("optical_table",pos,rot);
  
  ReflectionProperties  refl; 
  ColourProperties      colo;

  refl.SetReflectionCoefficient(1.0);
  colo.set_RGB_0to255(200,128,128);

  SurfaceProperties mirror_surface;
  mirror_surface.set_color(&colo);
  mirror_surface.set_reflection(&refl);

  pos.set(0.0,0.0,0.0);
  rot.set(0.0,0.0,0.0);
  
  //------------mirror 1----------------
  Plane mirror1;
  mirror1.set_frame("mirror_1",pos,rot);
  mirror1.set_outer_surface(&mirror_surface);
  mirror1.set_inner_surface(&mirror_surface);  
  mirror1.set_plane_using_x_and_y_width(1.0, 1.0);
  
  //------------mirror 2----------------
  pos.set(0.0,0.0,1.0);
  rot.set(0.0,0.0,0.0);
  Plane mirror2;
  mirror2.set_frame("mirror_2",pos,rot);
  mirror2.set_outer_surface(&mirror_surface);
  mirror2.set_inner_surface(&mirror_surface); 
  mirror2.set_plane_using_x_and_y_width(1.0, 1.0);

  //----------declare relationships------------
  optical_table.set_mother_and_child(&mirror1);
  optical_table.set_mother_and_child(&mirror2);

  world.set_mother_and_child(&optical_table);

  //---post initialize the world to calculate all bounding spheres---
  world.setup_tree_based_on_mother_child_relations();

  //----------free orbit-----------------------
  
  //FreeOrbitCamera free(&world,&setup);
  //free.start_free_orbit();

  //-----------send Photon----------------------
  // the photon is starting in between the to mirrors
  // traveling to the upper mirror
  Vector3D Support(0.0,0.0,0.5);
  Vector3D direction(0.0,0.0,1.0);
  double wavelength = 433e-9;
  
  PseudoRandomNumberGenerator dice;
  PropagationEnvironment environment;
  environment.world_geometry = &world;
  environment.propagation_options = &setup;
  environment.random_engine = &dice;
  environment.assert_completeness();

  for(int i=0; i<1e2; i++)
  {
    Photon P(Support, direction, wavelength);

    P.propagate_in(&environment);

    //history.show();
    //EXPECT_EQ(number_of_bounces*1.0-0.5, history.get_accumulative_distance() );
    //EXPECT_EQ(number_of_bounces,history.size());
  }
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, Reflections){
/* This test is about the propagation process  
                                 \ mirror pos(0,0,0)
    Light Source ->------>------>-\      
    pos(-2,0,0)                   |\
    dir(+1,0,0)                   |
                                  |
                                  |
                                  |
                             _____\/_____
    o-------------> +X       absorber pos(0,2,0)
    | coordinate frame
    |
    |
    |
    |
    \/ +Y

*/                              
  GlobalSettings setup;

  Vector3D    pos(0,0,0);
  Rotation3D  rot(0,0,0);

  // create a test setup with two mirrors bouncing the photon
  CartesianFrame world("world",pos,rot);

  CartesianFrame optical_table("optical_table",pos,rot);
   
  pos.set(0.0,0.0,0.0);
  rot.set(0.0,0.0,0.0);
  
  //------------mirror----------------
  ReflectionProperties  mirror_reflection;
  mirror_reflection.SetReflectionCoefficient(
    "./test_scenery/Function1D/function1D_complete.xml"
  ); 
  pos.set(0.0,0.0,0.0);
  //90Deg in Y and 45DEG in Z
  rot.set(0.0,Deg2Rad(90.0),Deg2Rad(45.0));

  ColourProperties mirror_color;
  mirror_color.set_RGB_0to255(200,64,64);

  SurfaceProperties mirror_surface;
  mirror_surface.set_color(&mirror_color);
  mirror_surface.set_reflection(&mirror_reflection);

  Plane mirror;
  mirror.set_frame("mirror",pos,rot);
  mirror.set_outer_surface(&mirror_surface);
  mirror.set_inner_surface(&mirror_surface); 
  mirror.set_plane_using_x_and_y_width(1.0, 1.0);
  
  //------------absorber----------------
  ReflectionProperties  absorber_reflection;
  absorber_reflection.SetReflectionCoefficient(0.0);
  pos.set(0.0,+2.0,0.0);
  rot.set(Deg2Rad(90.0),0.0,0.0);

  
  ColourProperties absorber_color;
  absorber_color.set_RGB_0to255(50,50,50);

  SurfaceProperties absorber_surface;
  absorber_surface.set_color(&absorber_color);

  Plane absorber;
  absorber.set_frame("absorber",pos,rot);
  absorber.set_outer_surface(&absorber_surface);
  absorber.set_inner_surface(&absorber_surface); 
  absorber.set_plane_using_x_and_y_width(1.0, 1.0);

  //----------declare relationships------------
  optical_table.set_mother_and_child(&mirror);
  optical_table.set_mother_and_child(&absorber);

  world.set_mother_and_child(&optical_table);

  //---post initialize the world to calculate all bounding spheres---
  world.setup_tree_based_on_mother_child_relations();

  //----------free orbit-----------------------
  
  //FreeOrbitCamera free(&world,&setup);
  //free.start_free_orbit();
  
  //-----------send Photon----------------------
  // the photon is starting in between the to mirrors
  // traveling to the upper mirror
  Vector3D Support(-2.0,0.0,0.0);
  Vector3D direction(1.0,0.0,0.0);
  
  PseudoRandomNumberGenerator dice;

  ListOfPropagations LoP("my_test_propagation_list");

  for(int i=1; i<=1e5; i++)
  {
    // wavelength form 1nm to 1000nm
    double wavelength = double(i)*1e-2*1e-9;

    Photon *P;
    P = new Photon(Support,direction,wavelength);
    LoP.push_back(P);

    //ListOfInteractions history;
    //Photon P(Support,direction,wavelength);

    //P.propagate(&world,&history,&setup,&dice);

    //history.show();
    //EXPECT_EQ(number_of_bounces*1.0-0.5, history.get_accumulative_distance() );
    //EXPECT_EQ(number_of_bounces,history.Interactions.size());
  }

  LoP.propagate_in_world_with_settings(&world, &setup);
  
}