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
  EXPECT_EQ(1, pho.get_number_of_interactions_so_far() ); //creation is an interaction
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
  
  ReflectionProperties  refl(1.0); 
  Color      colo(200,128,128);

  pos.set(0.0,0.0,0.0);
  rot.set(0.0,0.0,0.0);
  
  //------------mirror 1----------------
  Plane mirror1;
  mirror1.set_frame("mirror_1",pos,rot);
  mirror1.set_outer_color(&colo);
  mirror1.set_inner_color(&colo);
  mirror1.set_outer_reflection(&refl);
  mirror1.set_inner_reflection(&refl); 
  mirror1.set_plane_using_x_and_y_width(1.0, 1.0);
  
  //------------mirror 2----------------
  pos.set(0.0,0.0,1.0);
  rot.set(0.0,0.0,0.0);
  Plane mirror2;
  mirror2.set_frame("mirror_2",pos,rot);
  mirror2.set_outer_color(&colo);
  mirror2.set_inner_color(&colo);
  mirror2.set_outer_reflection(&refl);
  mirror2.set_inner_reflection(&refl);
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

  const uint num_of_total_interactions = number_of_bounces + 1; //creation is 1

  for(int i=0; i<1; i++)
  {
    Photon P(Support, direction, wavelength);

    P.propagate_in(&environment);
    //std::cout << P;

    EXPECT_EQ(number_of_bounces*1.0-0.5, P.get_accumulative_distance() );
    EXPECT_EQ(num_of_total_interactions, P.get_number_of_interactions_so_far() );
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
  const double reflection_coefficient = 0.42;
  ReflectionProperties  mirror_reflection(reflection_coefficient);

  pos.set(0.0,0.0,0.0);
  //90Deg in Y and 45DEG in Z
  rot.set(0.0,Deg2Rad(90.0),Deg2Rad(45.0));

  Color mirror_color;
  mirror_color.set_RGB_0to255(200,64,64);

  Plane mirror;
  mirror.set_frame("mirror",pos,rot);
  mirror.set_outer_color(&mirror_color);
  mirror.set_inner_color(&mirror_color);
  mirror.set_outer_reflection(&mirror_reflection);
  mirror.set_inner_reflection(&mirror_reflection); 
  mirror.set_plane_using_x_and_y_width(1.0, 1.0);
  
  //------------absorber----------------
  ReflectionProperties  absorber_reflection(0.0);
  pos.set(0.0,+2.0,0.0);
  rot.set(Deg2Rad(90.0),0.0,0.0);

  
  Color absorber_color;
  absorber_color.set_RGB_0to255(50,50,50);

  Plane absorber;
  absorber.set_frame("absorber",pos,rot);
  absorber.set_outer_color(&absorber_color);
  absorber.set_inner_color(&absorber_color);
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

  for(int i=1; i<=1e4; i++) {
    // wavelength form 1nm to 1000nm
    double wavelength = double(i)*1e-2*1e-9;

    Photon *P;
    P = new Photon(Support, direction, wavelength);
    P->set_id(i);
    LoP.push_back(P);
  }

  LoP.propagate_in_world_with_settings(&world, &setup);

  EXPECT_NEAR(
    reflection_coefficient, 
    double(LoP.get_number_of_propagations_absorbed_in_object(&absorber))/
    double(LoP.get_number_of_propagations()),
    2e-2
  );
  //std::cout << LoP.get_print();
}