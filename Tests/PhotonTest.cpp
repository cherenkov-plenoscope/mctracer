// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "Core/Function/Function.h"
#include "Core/Photon.h"
#include "Core/PhotonAndFrame.h"
#include "Core/Photons.h"
#include "Core/PhysicalConstants.h"
#include "Core/random/random.h"
#include "Core/SurfaceEntity.h"
#include "Scenery/Primitive/Plane.h"
#include "Scenery/Primitive/RectangularBox.h"
#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h"
using std::vector;
using namespace relleums;

class PhotonTest : public ::testing::Test {};

TEST_F(PhotonTest, creation) {
    double wavelength = 433e-9;
    Photon pho(VEC3_ORIGIN, VEC3_UNIT_Z*1.337, wavelength);
    EXPECT_EQ(VEC3_UNIT_Z, pho.direction());
    EXPECT_EQ(VEC3_ORIGIN, pho.support());
    EXPECT_EQ(1.0, pho.direction().norm());
    EXPECT_EQ(wavelength, pho.get_wavelength());
    // creation is an interaction
    EXPECT_EQ(1u, pho.get_number_of_interactions_so_far());
}

TEST_F(PhotonTest, reject_negative_wavelength) {
    EXPECT_THROW(
        Photon pho(VEC3_ORIGIN, VEC3_UNIT_X, 0.0),
        std::invalid_argument);
    EXPECT_THROW(
        Photon pho(VEC3_ORIGIN, VEC3_UNIT_X, -1.0),
        std::invalid_argument);
}

TEST_F(PhotonTest, PropagationSimpleGeometry) {
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    int number_of_bounces = 42;
    setup.max_number_of_interactions_per_photon = number_of_bounces;
    // create a test setup with two mirrors bouncing the photon
    Frame world;
    world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);
    Frame* optical_table = world.append<Plane>();
    optical_table->set_name_pos_rot("optical_table", VEC3_ORIGIN, ROT3_UNITY);
    function::Func1 refl(
        {
            {200e-9, 1},
            {1200e-9, 1}
        });

    Color colo(200, 128, 128);

    // ------------mirror 1----------------
    Plane* mirror1 = optical_table->append<Plane>();
    mirror1->set_name_pos_rot("mirror_1", VEC3_ORIGIN, ROT3_UNITY);
    mirror1->set_outer_color(&colo);
    mirror1->set_inner_color(&colo);
    mirror1->set_outer_reflection(&refl);
    mirror1->set_inner_reflection(&refl);
    mirror1->set_x_y_width(1.0, 1.0);

    // ------------mirror 2----------------
    Plane* mirror2 = optical_table->append<Plane>();
    mirror2->set_name_pos_rot("mirror_2", Vec3(0.0, 0.0, 1.0), ROT3_UNITY);
    mirror2->set_outer_color(&colo);
    mirror2->set_inner_color(&colo);
    mirror2->set_outer_reflection(&refl);
    mirror2->set_inner_reflection(&refl);
    mirror2->set_x_y_width(1.0, 1.0);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();
    // FlyingCamera free(&world,&setup);
    // free.start_free_orbit();

    // -----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    Vec3 Support(0.0, 0.0, 0.5);
    Vec3 direction(0.0, 0.0, 1.0);
    double wavelength = 433e-9;
    random::Mt19937 prng(random::ZERO_SEED);
    PropagationEnvironment environment;
    environment.root_frame = &world;
    environment.config = &setup;
    environment.prng = &prng;
    // creation is 1 interaction itself
    const unsigned int num_of_total_interactions = number_of_bounces + 1;
    for (int i = 0; i < 1; i++) {
        Photon P(Support, direction, wavelength);
        PhotonAndFrame::Propagator(&P, environment);
        EXPECT_EQ(
            number_of_bounces*1.0-0.5,
            P.get_accumulative_distance());
        EXPECT_EQ(
            num_of_total_interactions,
            P.get_number_of_interactions_so_far());
    }
}

TEST_F(PhotonTest, Reflections) {
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
    // create a test setup with two mirrors bouncing the photon
    Frame world;
    world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);
    Frame* optical_table = world.append<Plane>();
    optical_table->set_name_pos_rot("optical_table", VEC3_ORIGIN, ROT3_UNITY);

    // ------------mirror----------------
    const double reflection_coefficient = 0.42;
    function::Func1 mirror_reflection(
        {
            {200e-9, reflection_coefficient},
            {1200e-9, reflection_coefficient}
        });

    Color mirror_color;
    mirror_color = Color(200, 64, 64);
    Plane* mirror = optical_table->append<Plane>();
    mirror->set_name_pos_rot(
        "mirror",
        VEC3_ORIGIN,
        Rot3(0, deg2rad(90), deg2rad(45)));
    mirror->set_outer_color(&mirror_color);
    mirror->set_inner_color(&mirror_color);
    mirror->set_outer_reflection(&mirror_reflection);
    mirror->set_inner_reflection(&mirror_reflection);
    mirror->set_x_y_width(1.0, 1.0);

    // ------------absorber----------------
    Color absorber_color(50, 50, 50);
    Plane* absorber = optical_table->append<Plane>();
    absorber->set_name_pos_rot(
        "absorber",
        Vec3(0, 2, 0),
        Rot3(deg2rad(90), 0, 0));
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    PhotonSensor::Sensor absorber_sensor(0, absorber);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&absorber_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world,&setup);
    // free.start_free_orbit();

    // -----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    Vec3 Support(-2.0, 0.0, 0.0);
    Vec3 direction = VEC3_UNIT_X;
    random::Mt19937 prng(random::ZERO_SEED);
    vector<Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        // wavelength form 1nm to 1000nm
        double wavelength = static_cast<double>(i)/num_phot*100e-9+250e-9;
        Photon P(Support, direction, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    const double ph_reached_sensor =
        absorber_sensor.photon_arrival_history.size();
    const double ph_emitted = photons.size();
    EXPECT_NEAR(
        reflection_coefficient,
        ph_reached_sensor/ph_emitted,
        2e-2);
}

TEST_F(PhotonTest, Refraction) {
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    // create a test setup with two planes and high refractive index in between
    Frame world;
    world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);
    function::Func1 water_refraction(
        {
            {200e-9, 1.33},
            {1200e-9, 1.33}
        });

    // ------------ box ---------------
    Color entrance_surface_color(200, 64, 64);
    RectangularBox* box = world.append<RectangularBox>();
    box->set_name_pos_rot(
        "box",
        Vec3(0.0, 0.0, 1.0),
        Rot3(0.0, 0.0, 0.0));
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    // ------------absorber----------------
    Color absorber_color(50, 50, 50);
    Plane* absorber = world.append<Plane>();
    absorber->set_name_pos_rot(
        "absorber",
        Vec3(0.0, 0.0, 3.0),
        ROT3_UNITY);
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    PhotonSensor::Sensor absorber_sensor(sensor_id, absorber);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&absorber_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world, &setup);

    // -----------send Photon----------------------
    double wavelength = 433.0e-9;
    random::Mt19937 prng(random::ZERO_SEED);
    vector<Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        Photon P(VEC3_ORIGIN, VEC3_UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    // 5% fresnell reflection
    EXPECT_NEAR(
        0.95,
        static_cast<double>(absorber_sensor.photon_arrival_history.size())/
        static_cast<double>(num_phot),
        2e-2);
    const double travel_time = (2.0 + 1.33*1.0)/VACUUM_SPPED_OF_LIGHT;
    EXPECT_NEAR(
        travel_time,
        PhotonSensor::arrival_time_mean(absorber_sensor.photon_arrival_history),
        1e-10);
}

TEST_F(PhotonTest, absorbtion_in_medium) {
    PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    // create a test setup with two planes and high refractive index in between
    Frame world;
    world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);
    function::Func1 free_half_path(
        {
            {200e-9, 1.0},
            {1200e-9, 1.0}
        });

    function::Func1 water_refraction(
        {
            {200e-9, 1.33},
            {1200e-9, 1.33}
        });

    // ------------ box ---------------
    Color entrance_surface_color(200, 64, 64);
    RectangularBox* box = world.append<RectangularBox>();
    box->set_name_pos_rot(
        "box",
        Vec3(0.0, 0.0, 1.0),
        Rot3(0.0, 0.0, 0.0));
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_absorption(&free_half_path);
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    // ------------collector----------------
    Color absorber_color(50, 50, 50);
    Plane* collector = world.append<Plane>();
    collector->set_name_pos_rot(
        "collector",
        Vec3(0.0, 0.0, 3.0),
        ROT3_UNITY);
    collector->set_outer_color(&absorber_color);
    collector->set_inner_color(&absorber_color);
    collector->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    PhotonSensor::Sensor collector_sensor(sensor_id, collector);
    std::vector<PhotonSensor::Sensor*> sensors_vector = {&collector_sensor};
    PhotonSensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world, &setup);

    // -----------send Photons----------------------
    double wavelength = 433.0e-9;
    random::Mt19937 prng(random::ZERO_SEED);
    std::vector<Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        Photon P(VEC3_ORIGIN, VEC3_UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    EXPECT_NEAR(
        0.367,
        static_cast<double>(collector_sensor.photon_arrival_history.size())/
        static_cast<double>(num_phot),
        2e-2);
}
