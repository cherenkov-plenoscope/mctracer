// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "merlict/merlict.h"
#include "catch.hpp"
namespace ml = merlict;


TEST_CASE("PhotonTest: creation", "[merlict]") {
    double wavelength = 433e-9;
    ml::Photon pho(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z*1.337, wavelength);
    CHECK(pho.direction() == ml::VEC3_UNIT_Z);
    CHECK(pho.support() == ml::VEC3_ORIGIN);
    CHECK(pho.direction().norm() == 1.0);
    CHECK(pho.get_wavelength() == wavelength);
    // creation is an interaction
    CHECK(pho.get_num_interactions_so_far() == 1u);
}

TEST_CASE("PhotonTest: reject_negative_wavelength", "[merlict]") {
    CHECK_THROWS_AS(ml::Photon(ml::VEC3_ORIGIN, ml::VEC3_UNIT_X, 0.0), std::invalid_argument);
    CHECK_THROWS_AS(ml::Photon(ml::VEC3_ORIGIN, ml::VEC3_UNIT_X, -1.0), std::invalid_argument);
}

TEST_CASE("PhotonTest: PropagationSimpleGeometry", "[merlict]") {
    ml::PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    int num_bounces = 42;
    setup.max_num_interactions_per_photon = num_bounces;
    // create a test setup with two mirrors bouncing the photon
    ml::Frame world;
    world.set_name_pos_rot("world", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::Frame* optical_table = world.append<ml::Plane>();
    optical_table->set_name_pos_rot("optical_table", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::function::Func1 refl(
        {
            {200e-9, 1},
            {1200e-9, 1}
        });

    ml::Color colo(200, 128, 128);

    // ------------mirror 1----------------
    ml::Plane* mirror1 = optical_table->append<ml::Plane>();
    mirror1->set_name_pos_rot("mirror_1", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    mirror1->set_outer_color(&colo);
    mirror1->set_inner_color(&colo);
    mirror1->set_outer_reflection(&refl);
    mirror1->set_inner_reflection(&refl);
    mirror1->set_x_y_width(1.0, 1.0);

    // ------------mirror 2----------------
    ml::Plane* mirror2 = optical_table->append<ml::Plane>();
    mirror2->set_name_pos_rot("mirror_2", ml::Vec3(0.0, 0.0, 1.0), ml::ROT3_UNITY);
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
    ml::Vec3 Support(0.0, 0.0, 0.5);
    ml::Vec3 direction(0.0, 0.0, 1.0);
    double wavelength = 433e-9;
    ml::random::Mt19937 prng(0u);
    ml::PropagationEnvironment environment;
    environment.root_frame = &world;
    environment.config = &setup;
    environment.prng = &prng;
    // creation is 1 interaction itself
    const unsigned int num_of_total_interactions = num_bounces + 1;
    for (int i = 0; i < 1; i++) {
        ml::Photon P(Support, direction, wavelength);
        ml::PhotonAndFrame::Propagator(&P, environment);
        CHECK(P.get_accumulative_distance() == num_bounces*1.0-0.5);
        CHECK(P.get_num_interactions_so_far() == num_of_total_interactions);
    }
}

TEST_CASE("PhotonTest: Reflections", "[merlict]") {
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
    ml::PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    // create a test setup with two mirrors bouncing the photon
    ml::Frame world;
    world.set_name_pos_rot("world", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::Frame* optical_table = world.append<ml::Plane>();
    optical_table->set_name_pos_rot("optical_table", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    // ------------mirror----------------
    const double reflection_coefficient = 0.42;
    ml::function::Func1 mirror_reflection(
        {
            {200e-9, reflection_coefficient},
            {1200e-9, reflection_coefficient}
        });

    ml::Color mirror_color(200, 64, 64);
    ml::Plane* mirror = optical_table->append<ml::Plane>();
    mirror->set_name_pos_rot(
        "mirror",
        ml::VEC3_ORIGIN,
        ml::Rot3(0, ml::deg2rad(90), ml::deg2rad(45)));
    mirror->set_outer_color(&mirror_color);
    mirror->set_inner_color(&mirror_color);
    mirror->set_outer_reflection(&mirror_reflection);
    mirror->set_inner_reflection(&mirror_reflection);
    mirror->set_x_y_width(1.0, 1.0);

    // ------------absorber----------------
    ml::Color absorber_color(50, 50, 50);
    ml::Plane* absorber = optical_table->append<ml::Plane>();
    absorber->set_name_pos_rot(
        "absorber",
        ml::Vec3(0, 2, 0),
        ml::Rot3(ml::deg2rad(90), 0, 0));
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    ml::sensor::Sensor absorber_sensor(0, absorber);
    std::vector<ml::sensor::Sensor*> sensors_vector = {&absorber_sensor};
    ml::sensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world,&setup);
    // free.start_free_orbit();

    // -----------send Photon----------------------
    // the photon is starting in between the to mirrors
    // traveling to the upper mirror
    ml::Vec3 Support(-2.0, 0.0, 0.0);
    ml::Vec3 direction = ml::VEC3_UNIT_X;
    ml::random::Mt19937 prng(0u);
    std::vector<ml::Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        // wavelength form 1nm to 1000nm
        double wavelength = static_cast<double>(i)/num_phot*100e-9+250e-9;
        ml::Photon P(Support, direction, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    ml::Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    const double ph_reached_sensor =
        absorber_sensor.photon_arrival_history.size();
    const double ph_emitted = photons.size();
    CHECK(reflection_coefficient == Approx(ph_reached_sensor/ph_emitted).margin(2e-2));
}

TEST_CASE("PhotonTest: Refraction", "[merlict]") {
    ml::PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    // create a test setup with two planes and high refractive index in between
    ml::Frame world;
    world.set_name_pos_rot("world", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::function::Func1 water_refraction(
        {
            {200e-9, 1.33},
            {1200e-9, 1.33}
        });

    // ------------ box ---------------
    ml::Color entrance_surface_color(200, 64, 64);
    ml::RectangularBox* box = world.append<ml::RectangularBox>();
    box->set_name_pos_rot(
        "box",
        ml::Vec3(0.0, 0.0, 1.0),
        ml::Rot3(0.0, 0.0, 0.0));
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    // ------------absorber----------------
    ml::Color absorber_color(50, 50, 50);
    ml::Plane* absorber = world.append<ml::Plane>();
    absorber->set_name_pos_rot(
        "absorber",
        ml::Vec3(0.0, 0.0, 3.0),
        ml::ROT3_UNITY);
    absorber->set_outer_color(&absorber_color);
    absorber->set_inner_color(&absorber_color);
    absorber->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    ml::sensor::Sensor absorber_sensor(sensor_id, absorber);
    std::vector<ml::sensor::Sensor*> sensors_vector = {&absorber_sensor};
    ml::sensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world, &setup);

    // -----------send Photon----------------------
    double wavelength = 433.0e-9;
    ml::random::Mt19937 prng(0u);
    std::vector<ml::Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        ml::Photon P(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    ml::Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    // 5% fresnell reflection
    CHECK(0.95 == Approx(static_cast<double>(absorber_sensor.photon_arrival_history.size())/static_cast<double>(num_phot)).margin(2e-2));
    const double travel_time = (2.0 + 1.33*1.0)/ml::VACUUM_SPPED_OF_LIGHT;
    CHECK(travel_time == Approx(ml::sensor::arrival_time_mean(absorber_sensor.photon_arrival_history)).margin(1e-10));
}

TEST_CASE("PhotonTest: absorbtion_in_medium", "[merlict]") {
    ml::PropagationConfig setup;
    setup.use_multithread_when_possible = false;
    // create a test setup with two planes and high refractive index in between
    ml::Frame world;
    world.set_name_pos_rot("world", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::function::Func1 free_half_path(
        {
            {200e-9, 1.0},
            {1200e-9, 1.0}
        });

    ml::function::Func1 water_refraction(
        {
            {200e-9, 1.33},
            {1200e-9, 1.33}
        });

    // ------------ box ---------------
    ml::Color entrance_surface_color(200, 64, 64);
    ml::RectangularBox* box = world.append<ml::RectangularBox>();
    box->set_name_pos_rot(
        "box",
        ml::Vec3(0.0, 0.0, 1.0),
        ml::Rot3(0.0, 0.0, 0.0));
    box->set_outer_color(&entrance_surface_color);
    box->set_inner_color(&entrance_surface_color);
    box->set_inner_absorption(&free_half_path);
    box->set_inner_refraction(&water_refraction);
    box->set_xyz_width(1.0, 1.0, 1.0);

    // ------------collector----------------
    ml::Color absorber_color(50, 50, 50);
    ml::Plane* collector = world.append<ml::Plane>();
    collector->set_name_pos_rot(
        "collector",
        ml::Vec3(0.0, 0.0, 3.0),
        ml::ROT3_UNITY);
    collector->set_outer_color(&absorber_color);
    collector->set_inner_color(&absorber_color);
    collector->set_x_y_width(1.0, 1.0);
    unsigned int sensor_id = 0;
    ml::sensor::Sensor collector_sensor(sensor_id, collector);
    std::vector<ml::sensor::Sensor*> sensors_vector = {&collector_sensor};
    ml::sensor::Sensors sensors(sensors_vector);

    // ---post initialize the world to calculate all bounding spheres---
    world.init_tree_based_on_mother_child_relations();

    // ----------free orbit-----------------------
    // FlyingCamera free(&world, &setup);

    // -----------send Photons----------------------
    double wavelength = 433.0e-9;
    ml::random::Mt19937 prng(0u);
    std::vector<ml::Photon> photons;
    double num_phot = 1e4;
    for (int i = 1; i <= num_phot; i++) {
        ml::Photon P(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z, wavelength);
        P.set_simulation_truth_id(i);
        photons.push_back(P);
    }
    ml::Photons::propagate_photons_in_scenery_with_settings(
        &photons, &world, &setup, &prng);
    sensors.assign_photons(&photons);
    CHECK(0.367 == Approx(static_cast<double>(collector_sensor.photon_arrival_history.size())/static_cast<double>(num_phot)).margin(2e-2));
}
