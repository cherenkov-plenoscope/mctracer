//<#include "Cameras/FlyingCamera.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/Photon.h"
#include "Core/PhotonAndFrame.h"
#include "Core/Photons.h"
#include "Core/PhysicalConstants.h"
#include "Core/Random/Random.h"
#include "Core/SurfaceEntity.h"
#include "Scenery/Primitive/Plane.h"
#include "Scenery/Primitive/RectangularBox.h"
#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h" 
#include <iostream> 
#include <math.h>
#include <string>
#include <vector>
using std::vector;

class PhotonTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(PhotonTest, creation) {
    
    double wavelength = 433e-9;
    Photon pho(Vec3::ORIGIN, Vec3::UNIT_Z*1.337, wavelength);

    EXPECT_EQ(Vec3::UNIT_Z, pho.get_direction());
    EXPECT_EQ(Vec3::ORIGIN, pho.get_support());
    EXPECT_EQ(1.0, pho.get_direction().norm());
    EXPECT_EQ(wavelength, pho.get_wavelength());
    //creation is an interaction
    EXPECT_EQ(1u, pho.get_number_of_interactions_so_far()); 
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, reject_negative_wavelength) {

    EXPECT_THROW(
        Photon pho(Vec3::ORIGIN, Vec3::UNIT_X, 0.0),
        std::invalid_argument
    ); 

    EXPECT_THROW(
        Photon pho(Vec3::ORIGIN, Vec3::UNIT_X, -1.0),
        std::invalid_argument
    );   
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, PropagationSimpleGeometry){

    PropagationConfig setup;
    setup.use_multithread_when_possible = false;

    int number_of_bounces = 42;
    setup.max_number_of_interactions_per_photon = number_of_bounces;

    Vec3  pos(0,0,0);
    Rot3  rot(0,0,0);

    // create a test setup with two mirrors bouncing the photon
    Frame world;
    world.set_name_pos_rot("world",pos,rot);
    Frame* optical_table = world.append<Plane>();
    optical_table->set_name_pos_rot("optical_table",pos,rot);
    
    Function::Constant refl(1.0, Function::Limits(200e-9, 1200e-9)); 
    Color      colo(200,128,128);

    pos.set(0.0,0.0,0.0);
    rot.set(0.0,0.0,0.0);
    
    //------------mirror 1----------------
    Plane* mirror1 = optical_table->append<Plane>();
    mirror1->set_name_pos_rot("mirror_1",pos,rot);
    mirror1->set_outer_color(&colo);
    mirror1->set_inner_color(&colo);
    mirror1->set_outer_reflection(&refl);
    mirror1->set_inner_reflection(&refl); 
    mirror1->set_x_y_width(1.0, 1.0);
    
    //------------mirror 2----------------
    pos.set(0.0,0.0,1.0);
    rot.set(0.0,0.0,0.0);
    Plane* mirror2 = optical_table->append<Plane>();
    mirror2->set_name_pos_rot("mirror_2",pos,rot);
    mirror2->set_outer_color(&colo);
    mirror2->set_inner_color(&colo);
    mirror2->set_outer_reflection(&refl);
    mirror2->set_inner_reflection(&refl);
    mirror2->set_x_y_width(1.0, 1.0);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------
    
    //FlyingCamera free(&world,&setup);
    //free.start_free_orbit();

    //-----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    Vec3 Support(0.0,0.0,0.5);
    Vec3 direction(0.0,0.0,1.0);
    double wavelength = 433e-9;
    
    Random::Mt19937 dice(Random::ZERO_SEED);
    PropagationEnvironment environment;
    environment.world_geometry = &world;
    environment.propagation_options = &setup;
    environment.random_engine = &dice;

    //creation is 1 interaction itself
    const unsigned int num_of_total_interactions = number_of_bounces + 1; 

    for(int i=0; i<1; i++) {
        Photon P(Support, direction, wavelength);

        PhotonAndFrame::Propagator(&P, environment);
        
        EXPECT_EQ(
            number_of_bounces*1.0-0.5, 
            P.get_accumulative_distance() 
        );

        EXPECT_EQ(
            num_of_total_interactions, 
            P.get_number_of_interactions_so_far()
        );
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
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;

    Vec3 pos = Vec3::ORIGIN;
    Rot3 rot = Rot3::null;

    // create a test setup with two mirrors bouncing the photon
    Frame world;
    world.set_name_pos_rot("world",pos,rot);
    Frame* optical_table = world.append<Plane>();
    optical_table->set_name_pos_rot("optical_table",pos,rot);

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

    Plane* mirror = optical_table->append<Plane>();
    mirror->set_name_pos_rot("mirror",pos,rot);
    mirror->set_outer_color(&mirror_color);
    mirror->set_inner_color(&mirror_color);
    mirror->set_outer_reflection(&mirror_reflection);
    mirror->set_inner_reflection(&mirror_reflection); 
    mirror->set_x_y_width(1.0, 1.0);

    //------------absorber----------------
    pos.set(0.0,+2.0,0.0);
    rot.set(Deg2Rad(90.0),0.0,0.0);

    Color absorber_color(50,50,50);

    Plane* absorber = optical_table->append<Plane>();
    absorber->set_name_pos_rot("absorber",pos,rot);
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    PhotonSensor::Sensor absorber_sensor(0, absorber);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&absorber_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------
    //FlyingCamera free(&world,&setup);
    //free.start_free_orbit();

    //-----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    Vec3 Support(-2.0, 0.0, 0.0);
    Vec3 direction = Vec3::UNIT_X;

    Random::Mt19937 prng(Random::ZERO_SEED);

    vector<Photon> photons;

    double num_phot = 1e4;
    for(int i=1; i<=num_phot; i++) {
        // wavelength form 1nm to 1000nm
        
        double wavelength = double(i)/num_phot*100e-9+250e-9;

        Photon P(Support, direction, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }

    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng
    );

    sensors.assign_photons(&photons);

    double ph_reached_sensor = absorber_sensor.get_arrival_table().size();
    double ph_emitted = photons.size();

    EXPECT_NEAR(
        reflection_coefficient, 
        ph_reached_sensor/ph_emitted,
        2e-2
    );
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, Refraction){
                           
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;

    // create a test setup with two planes and high refractive index in between
    Frame world;
    world.set_name_pos_rot("world", Vec3::ORIGIN, Rot3::null);

    Function::Constant water_refraction(
            1.33,
            Function::Limits(200e-9, 1200e-9)
    );

    //------------ box ---------------
    Color entrance_surface_color(200,64,64);

    RectangularBox* box = world.append<RectangularBox>();
    box->set_name_pos_rot(
        "box", 
        Vec3(0.0, 0.0, 1.0), 
        Rot3(0.0, 0.0, 0.0)
    );
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    //------------absorber----------------
    Color absorber_color(50,50,50);

    Plane* absorber = world.append<Plane>();
    absorber->set_name_pos_rot(
        "absorber", 
        Vec3(0.0, 0.0, 3.0),
        Rot3::null
    );
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    PhotonSensor::Sensor absorber_sensor(sensor_id, absorber);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&absorber_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------
    //FlyingCamera free(&world, &setup);

    //-----------send Photon----------------------
    double wavelength = 433.0e-9;

    Random::Mt19937 prng(Random::ZERO_SEED);
    vector<Photon> photons;

    double num_phot = 1e4;
    for(int i=1; i<=num_phot; i++) {
        
        Photon P(Vec3::ORIGIN, Vec3::UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }

    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng
    );

    sensors.assign_photons(&photons);

    // 5% fresnell reflection
    EXPECT_NEAR(
        0.95, 
        double(absorber_sensor.get_arrival_table().size())/double(num_phot),
        2e-2
    );

    double travel_time = (2.0 + 1.33*1.0)/
        PhysicalConstants::VACUUM_SPPED_OF_LIGHT;

    EXPECT_NEAR(
        travel_time, 
        absorber_sensor.arrival_time_mean(),
        1e-10
    );
}
//------------------------------------------------------------------------------
TEST_F(PhotonTest, absorbtion_in_medium){
                           
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;

    // create a test setup with two planes and high refractive index in between
    Frame world;
    world.set_name_pos_rot("world", Vec3::ORIGIN, Rot3::null);

    Function::Constant free_half_path(
        1.0,
        Function::Limits(200e-9, 1200e-9)
    );

    Function::Constant water_refraction(
        1.33,
        Function::Limits(200e-9, 1200e-9)
    );

    //------------ box ---------------
    Color entrance_surface_color(200,64,64);

    RectangularBox* box = world.append<RectangularBox>();
    box->set_name_pos_rot(
        "box", 
        Vec3(0.0, 0.0, 1.0), 
        Rot3(0.0, 0.0, 0.0)
    );
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_absorption(&free_half_path); 
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    //------------collector----------------
    Color absorber_color(50,50,50);

    Plane* collector = world.append<Plane>();
    collector->set_name_pos_rot(
        "collector", 
        Vec3(0.0, 0.0, 3.0),
        Rot3::null
    );
    collector->set_outer_color(&absorber_color);
    collector->set_inner_color(&absorber_color);
    collector->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    PhotonSensor::Sensor collector_sensor(sensor_id, collector);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&collector_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    //---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    //----------free orbit-----------------------
    //FlyingCamera free(&world, &setup);

    //-----------send Photons----------------------
    double wavelength = 433.0e-9;
    Random::Mt19937 prng(Random::ZERO_SEED);
    std::vector<Photon> photons;

    double num_phot = 1e4;
    for(int i=1; i<=num_phot; i++) {
        
        Photon P(Vec3::ORIGIN, Vec3::UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }

    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng
    );

    sensors.assign_photons(&photons);

    EXPECT_NEAR(
        0.367, 
        double(collector_sensor.get_arrival_table().size())/double(num_phot),
        2e-2
    );
}