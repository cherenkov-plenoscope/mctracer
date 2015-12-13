#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Photon.h"
#include "Core/SurfaceEntity.h"
#include "../Plane.h"
#include "Cameras/FreeOrbitCamera.h"
#include "Core/PseudoRandomNumberGenerator.h"
#include "Core/Photons.h"
#include "Core/Function/ConstantFunction.h"
#include "PhotonSensor/PhotonSensor.h" 
#include "Core/PhysicalConstants.h"

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
TEST_F(PhotonTest, reject_negative_wavelength) {

    EXPECT_THROW(
        Photon pho(Vector3D::null, Vector3D::unit_x, 0.0),
        Photon::BadWaveLength
    ); 

    EXPECT_THROW(
        Photon pho(Vector3D::null, Vector3D::unit_x, -1.0),
        Photon::BadWaveLength
    );   
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

    const uint num_of_total_interactions = number_of_bounces + 1; //creation is 1

    for(int i=0; i<1; i++)
    {
        Photon P(Support, direction, wavelength);

        P.propagate_in(&environment);
        
        EXPECT_EQ(number_of_bounces*1.0-0.5, P.get_accumulative_distance() );
        EXPECT_EQ(num_of_total_interactions, P.get_number_of_interactions_so_far() );
        P.delete_history();
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
    mirror_color = Color(200,64,64);

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

    Color absorber_color(50,50,50);

    Plane absorber;
    absorber.set_name_pos_rot("absorber",pos,rot);
    absorber.set_outer_color(&absorber_color);
    absorber.set_inner_color(&absorber_color);
    absorber.set_x_y_width(1.0, 1.0);
    PhotonSensor::PerfectSensor absorber_sensor(0, &absorber);
    std::vector<PhotonSensor::Sensor*> sensors = {&absorber_sensor};
    PhotonSensors::sort_photon_sensors_based_on_frames(&sensors);

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

    Photons::propagate_photons_in_world_with_settings(
        photon_bunch, &world, &setup
    );

    PhotonSensors::assign_photons_to_sensors(photon_bunch, &sensors);

    EXPECT_NEAR(
        reflection_coefficient, 
        double(absorber_sensor.get_arrival_table().size())/double(photon_bunch->size()),
        2e-2
    );

    Photons::delete_photons_and_history(photon_bunch);
    delete photon_bunch;    
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, Refraction){
                           
    TracerSettings setup;

    // create a test setup with two planes and high refractive index in between
    Frame world("world", Vector3D::null, Rotation3D::null);

    Function::Constant water_refraction(
            1.33,
            Function::Limits(200e-9, 1200e-9)
    );

    //------------entrance surface---------------
    Color entrance_surface_color(200,64,64);

    Plane entrance_surface;
    entrance_surface.set_name_pos_rot(
        "entrance_surface", 
        Vector3D(0.0, 0.0, 1.0), 
        Rotation3D(0.0, M_PI, 0.0)
    );
    entrance_surface.set_outer_color(&entrance_surface_color);
    entrance_surface.set_inner_color(&entrance_surface_color);
    entrance_surface.set_inner_refraction(&water_refraction); 
    entrance_surface.set_x_y_width(1.0, 1.0);

    //------------exit surface---------------
    Color exit_surface_color(200,64,64);

    Plane exit_surface;
    exit_surface.set_name_pos_rot(
        "exit_surface", 
        Vector3D(0.0, 0.0, 2.0),
        Rotation3D::null
    );
    exit_surface.set_outer_color(&exit_surface_color);
    exit_surface.set_inner_color(&exit_surface_color);
    exit_surface.set_inner_refraction(&water_refraction); 
    exit_surface.set_x_y_width(1.0, 1.0);

    //------------absorber----------------
    Color absorber_color(50,50,50);

    Plane absorber;
    absorber.set_name_pos_rot(
        "absorber", 
        Vector3D(0.0, 0.0, 3.0),
        Rotation3D::null
    );
    absorber.set_outer_color(&absorber_color);
    absorber.set_inner_color(&absorber_color);
    absorber.set_x_y_width(1.0, 1.0);
    uint sensor_id = 0;
    PhotonSensor::PerfectSensor absorber_sensor(sensor_id, &absorber);
    std::vector<PhotonSensor::Sensor*> sensors = {&absorber_sensor};
    PhotonSensors::sort_photon_sensors_based_on_frames(&sensors);

    //----------declare relationships------------
    world.set_mother_and_child(&entrance_surface);
    world.set_mother_and_child(&exit_surface);
    world.set_mother_and_child(&absorber);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------
    FreeOrbitCamera free(&world, &setup);

    //-----------send Photon----------------------
    double wavelength = 433.0e-9;

    Random::Mt19937 dice(Random::zero_seed);
    std::vector<Photon*>* photon_bunch = new std::vector<Photon*>;

    double num_phot = 1e4;
    for(int i=1; i<=num_phot; i++) {
        
        Photon *P;
        P = new Photon(Vector3D::null, Vector3D::unit_z, wavelength);
        P->set_id(i);
        photon_bunch->push_back(P);
    }

    Photons::propagate_photons_in_world_with_settings(
        photon_bunch, &world, &setup
    );

    PhotonSensors::assign_photons_to_sensors(photon_bunch, &sensors);

    // 5% fresnell reflection
    EXPECT_NEAR(
        0.95, 
        double(absorber_sensor.get_arrival_table().size())/double(num_phot),
        2e-2
    );

    double travel_time = (2.0 + 1.33*1.0)/PhysicalConstants::speed_of_light_in_vacuum;

    EXPECT_NEAR(
        travel_time, 
        get_mean_along_column(absorber_sensor.get_arrival_table(), 4),
        1e-10
    );

    Photons::delete_photons_and_history(photon_bunch);
    delete photon_bunch;
}