// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include "catch.hpp"
#include "merlict/merlict.h"
#include <iostream>
#include "viewer/FlyingCamera.h"

using namespace merlict;
using std::vector;

struct BiConvexLensTest {
    Scenery scenery;
    PropagationConfig settings;
    random::Mt19937 prng;
    PropagationEnvironment env;
    sensor::Sensors sensor_list;

    BiConvexLensTest() {
        set_up_settings();
        set_up_test_bench();

        prng.set_seed(0u);

        env.root_frame = &scenery.root;
        env.config = &settings;
        env.prng = &prng;
    }

    void set_up_settings() {
        settings.max_number_of_interactions_per_photon = 5;
    }

    void set_up_test_bench() {
        Frame* test_bench = scenery.root.append<Frame>();
        test_bench->set_name_pos_rot(
            "BiConvexLens_test_world",
            VEC3_ORIGIN,
            ROT3_UNITY);

        lens_maker::Config cfg;
        cfg.focal_length = 1.0;
        cfg.aperture_radius = 0.125;
        cfg.refractive_index = 1.49;

        BiConvexLens* lens = test_bench->append<BiConvexLens>();
        lens->set_name_pos_rot(
            "little_lens",
            VEC3_ORIGIN,
            ROT3_UNITY);

        scenery.colors.add("lens_color", Color(255, 128, 128));
        scenery.functions.add(
            "refraction_vs_wavelength",
            function::Func1(
                {
                    {200e-9, 1.49},
                    {1200e-9, 1.49}
                }));

        lens->set_outer_color(scenery.colors.get("lens_color"));
        lens->set_inner_color(scenery.colors.get("lens_color"));
        lens->set_inner_refraction(scenery.functions.get("refraction_vs_wavelength"));
        lens->set_curvature_radius_and_aperture_radius(
            lens_maker::get_curvature_radius(cfg),
            cfg.aperture_radius);
        //-------------------------
        Disc* image_sensor = test_bench->append<Disc>();
        image_sensor->set_name_pos_rot(
            "sensor_disc",
            Vec3(0.0, 0.0, -1.0),
            ROT3_UNITY);

        scenery.colors.add("sensor_color", Color(128, 255, 128));
        image_sensor->set_outer_color(scenery.colors.get("sensor_color"));
        image_sensor->set_inner_color(scenery.colors.get("sensor_color"));
        image_sensor->set_radius(cfg.aperture_radius);

        scenery.sensors.add(0, image_sensor);
        sensor_list.init(scenery.sensors.sensors);
        //-------------------------

        scenery.root.init_tree_based_on_mother_child_relations();

        //visual::Config cfg;
        //visual::FlyingCamera(&scenery.root, &cfg);
    }
};

TEST_CASE("BiConvexLensTest: send_photon_frontal_into_lens", "[merlict]") {
    BiConvexLensTest lt;
    unsigned int total_propagations = 1e4;
    unsigned int number_of_photons_reaching_sensor_disc = 0;
    for (unsigned int i = 0; i < total_propagations; i++) {
        Photon blue_photon(Vec3(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), 433e-9);
        PhotonAndFrame::Propagator(&blue_photon, lt.env);
        // blue_photon.propagate_in(env);

        if (2.0 == blue_photon.get_accumulative_distance())
            number_of_photons_reaching_sensor_disc++;
    }

    CHECK(0.97 == Approx(static_cast<double>(number_of_photons_reaching_sensor_disc)/static_cast<double>(total_propagations)).margin(5.0e-2));
}

TEST_CASE("BiConvexLensTest: send_photons_frontal_into_lens_with_offset", "[merlict]") {
    BiConvexLensTest lt;

    // light source
    unsigned int number_of_photons_emitted = 1e4;
    random::Mt19937 prng(0);
    vector<Photon> photons = Photons::Source::parallel_towards_z_from_xy_disc(
        0.125,
        number_of_photons_emitted,
        &prng);

    HomTra3 Trafo;
    Trafo.set_transformation(
        Rot3(0.0, -deg2rad(180.0), 0.0),
        Vec3(0.0, 0.0, 1.0));

    Photons::transform_all_photons(Trafo, &photons);

    // photon propagation
    Photons::propagate_photons_in_scenery_with_settings(
        &photons,
        lt.env.root_frame,
        lt.env.config,
        &prng);

    // detect photons in sensors
    lt.sensor_list.clear_history();
    lt.sensor_list.assign_photons(&photons);

    REQUIRE(lt.sensor_list.by_frame.size() == 1);

    sensor::Sensor* sen = lt.sensor_list.at(0);

    CHECK(1.5e-3 == Approx(sensor::point_spread_std_dev(sen->photon_arrival_history)).margin(1e-3));

    CHECK(1.0 == Approx(static_cast<double>(sen->photon_arrival_history.size())/static_cast<double>(number_of_photons_emitted)).margin(10e-2));

    /*FlyingCamera free(
        env.root_frame,
        env.config
    );*/
}
