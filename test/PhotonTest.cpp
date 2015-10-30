#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Photon.h"
#include "Core/SurfaceEntity.h"
#include "../Plane.h"
#include "Cameras/FreeOrbitCamera.h"
#include "Core/PseudoRandomNumberGenerator.h"
#include "Core/PhotonBunch.h"
#include "Core/Function/ConstantFunction.h"

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
  EXPECT_EQ(1.0, pho.Direction().norm());
  EXPECT_EQ(wavelength, pho.get_wavelength());
  EXPECT_EQ(1, pho.get_number_of_interactions_so_far() ); //creation is an interaction
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, PropagationSimpleGeometry){

  TracerSettings setup;

  int number_of_bounces = 42;
  setup.set_max_number_of_reflections(number_of_bounces);

  Vector3D    pos(0,0,0);
  Rotation3D  rot(0,0,0);

  // create a test setup with two mirrors bouncing the photon
  Frame world("world",pos,rot);

  Frame optical_table("optical_table",pos,rot);
  
  Function::Constant refl(1.0, Function::Limits(200e-9, 1200e-9)); 
  Color      colo(200,128,128);

  pos.set(0.0,0.0,0.0);
  rot.set(0.0,0.0,0.0);
  
  //------------mirror 1----------------
  Plane mirror1;
  mirror1.set_name_pos_rot("mirror_1",pos,rot);
  mirror1.set_outer_color(&colo);
  mirror1.set_inner_color(&colo);
  mirror1.set_outer_reflection(&refl);
  mirror1.set_inner_reflection(&refl); 
  mirror1.set_x_y_width(1.0, 1.0);
  
  //------------mirror 2----------------
  pos.set(0.0,0.0,1.0);
  rot.set(0.0,0.0,0.0);
  Plane mirror2;
  mirror2.set_name_pos_rot("mirror_2",pos,rot);
  mirror2.set_outer_color(&colo);
  mirror2.set_inner_color(&colo);
  mirror2.set_outer_reflection(&refl);
  mirror2.set_inner_reflection(&refl);
  mirror2.set_x_y_width(1.0, 1.0);

  //----------declare relationships------------
  optical_table.set_mother_and_child(&mirror1);
  optical_table.set_mother_and_child(&mirror2);

  world.set_mother_and_child(&optical_table);

  //---post initialize the world to calculate all bounding spheres---
  world.init_tree_based_on_mother_child_relations();

  //----------free orbit-----------------------
  
  //FreeOrbitCamera free(&world,&setup);
  //free.start_free_orbit();

  //-----------send Photon----------------------
  // the photon is starting in between the to mirrors
  // traveling to the upper mirror
  Vector3D Support(0.0,0.0,0.5);
  Vector3D direction(0.0,0.0,1.0);
  double wavelength = 433e-9;
  
  Random::Mt19937 dice(Random::zero_seed);
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
    TracerSettings setup;

    Vector3D    pos = Vector3D::null;
    Rotation3D  rot = Rotation3D::null;

    // create a test setup with two mirrors bouncing the photon
    Frame world("world",pos,rot);

    Frame optical_table("optical_table",pos,rot);

    pos.set(0.0,0.0,0.0);
    rot.set(0.0,0.0,0.0);

    //------------mirror----------------
    const double reflection_coefficient = 0.42;

    Function::Constant mirror_reflection(
        reflection_coefficient,
        Function::Limits(200e-9, 1200e-9)
    );

    pos.set(0.0,0.0,0.0);
    //90Deg in Y and 45DEG in Z
    rot.set(0.0,Deg2Rad(90.0),Deg2Rad(45.0));

    Color mirror_color;
    mirror_color.set_RGB_0to255(200,64,64);

    Plane mirror;
    mirror.set_name_pos_rot("mirror",pos,rot);
    mirror.set_outer_color(&mirror_color);
    mirror.set_inner_color(&mirror_color);
    mirror.set_outer_reflection(&mirror_reflection);
    mirror.set_inner_reflection(&mirror_reflection); 
    mirror.set_x_y_width(1.0, 1.0);
  
    //------------absorber----------------
    pos.set(0.0,+2.0,0.0);
    rot.set(Deg2Rad(90.0),0.0,0.0);


    Color absorber_color;
    absorber_color.set_RGB_0to255(50,50,50);

    Plane absorber;
    absorber.set_name_pos_rot("absorber",pos,rot);
    absorber.set_outer_color(&absorber_color);
    absorber.set_inner_color(&absorber_color);
    absorber.set_x_y_width(1.0, 1.0);

    //----------declare relationships------------
    optical_table.set_mother_and_child(&mirror);
    optical_table.set_mother_and_child(&absorber);

    world.set_mother_and_child(&optical_table);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------

    //FreeOrbitCamera free(&world,&setup);
    //free.start_free_orbit();

    //-----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    Vector3D Support(-2.0, 0.0, 0.0);
    Vector3D direction = Vector3D::unit_x;
  
    Random::Mt19937 dice(Random::zero_seed);

    std::vector<Photon*>* photon_bunch = new std::vector<Photon*>;

    double num_phot = 1e4;
    for(int i=1; i<=num_phot; i++) {
        // wavelength form 1nm to 1000nm
        
        double wavelength = double(i)/num_phot*100e-9+250e-9;

        Photon *P;
        P = new Photon(Support, direction, wavelength);
        P->set_id(i);
        photon_bunch->push_back(P);
    }

    PhotonBunch::propagate_photons_in_world_with_settings(
        photon_bunch, &world, &setup
    );

    EXPECT_NEAR(
        reflection_coefficient, 
        double(PhotonBunch::get_number_of_photnons_absorbed_in_object(photon_bunch, &absorber))/
        double(photon_bunch->size()),
        2e-2
    );
}