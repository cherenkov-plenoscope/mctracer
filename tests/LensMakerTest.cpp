// Copyright 2014 Sebastian A. Mueller
#include <algorithm>
#include <vector>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;
using std::vector;


TEST_CASE("lens_makerTest: check_sebastians_paper_and_pen_calculation", "[merlict]") {
    double expected_curvature_radius = 0.125;;
    ml::lens_maker::Config cfg;
    cfg.focal_length = 0.1335;
    cfg.aperture_radius = 0.071;
    cfg.refractive_index = 1.49;
    CHECK(expected_curvature_radius == Approx(ml::lens_maker::get_curvature_radius(cfg)).margin(expected_curvature_radius*3e-2));
}

TEST_CASE("lens_makerTest: lens_thicknes", "[merlict]") {
    double expected_curvature_radius = 0.125;
    double expected_thickness = 0.0445;
    ml::lens_maker::Config cfg;
    cfg.focal_length = 0.1335;
    cfg.aperture_radius = 0.071;
    cfg.refractive_index = 1.49;
    CHECK(expected_thickness == Approx(ml::lens_maker::get_lens_thickness_for_R_r(expected_curvature_radius, cfg.aperture_radius)).margin(expected_thickness*3e-2));
}

TEST_CASE("lens_makerTest: check_lensmaker_on_optical_table_with_lens", "[merlict]") {
    // Hello lens_maker,
    ml::lens_maker::Config cfg;
    // we want a lens with
    cfg.focal_length = 1.0;  // 0.1335;
    // made out of pmma plastic with
    cfg.refractive_index = 1.49;
    // and an outer aperture radius of
    cfg.aperture_radius = 0.071;
    // please tell us the needed curvature radius.

    double lens_curvature_radius = ml::lens_maker::get_curvature_radius(cfg);

    // ok lets test it...
    const ml::Color* lens_col = &ml::COLOR_GRAY;
    const ml::Color* sensor_disc_col = &ml::COLOR_DARK_GRAY;
    const ml::function::Func1 refraction_vs_wavelength(
        {
            {200e-9, cfg.refractive_index},
            {1200e-9, cfg.refractive_index}
        });
    ml::random::Mt19937 prng(0u);
    unsigned int number_of_photons_per_run = 1000;
    vector<double> sigma_psf_vs_image_sensor_distance;
    vector<double> image_sensor_distances;
    for (
        double offset = -cfg.focal_length*0.3;
        offset < cfg.focal_length*0.3;
        offset = offset+cfg.focal_length*0.01
    ) {
        double image_sensor_disc_distance = cfg.focal_length + offset;

        // geometry
        ml::Frame optical_table;
        optical_table.set_name_pos_rot("table", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
        ml::BiConvexLensHexBound* lens =
            optical_table.append<ml::BiConvexLensHexBound>();
        lens->set_name_pos_rot("lens", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
        lens->set_outer_color(lens_col);
        lens->set_inner_color(lens_col);
        lens->set_inner_refraction(&refraction_vs_wavelength);
        lens->set_curvature_radius_and_outer_hex_radius(
            lens_curvature_radius,
            cfg.aperture_radius);
        ml::Disc* sensor_disc = optical_table.append<ml::Disc>();
        sensor_disc->set_name_pos_rot(
            "sensor_disc",
            ml::Vec3(0.0, 0.0, -image_sensor_disc_distance),
            ml::ROT3_UNITY);
        sensor_disc->set_outer_color(sensor_disc_col);
        sensor_disc->set_inner_color(sensor_disc_col);
        sensor_disc->set_radius(cfg.aperture_radius*0.85);
        ml::sensor::Sensor sensor(0, sensor_disc);
        vector<ml::sensor::Sensor*> sensor_vec = {&sensor};
        ml::sensor::Sensors sensor_list(sensor_vec);
        optical_table.init_tree_based_on_mother_child_relations();

        // light source
        vector<ml::Photon> photons =
            ml::Photons::Source::parallel_towards_z_from_xy_disc(
                cfg.aperture_radius*0.85,  // 0.85 inner hex radius
                number_of_photons_per_run,
                &prng);
        ml::HomTra3 Trafo;
        Trafo.set_transformation(
            ml::Rot3(0.0, -ml::deg2rad(180.0), 0.0),
            ml::Vec3(0.0, 0.0, 2.0));
        ml::Photons::transform_all_photons(Trafo, &photons);

        // propagation settings
        ml::PropagationConfig settings;
        settings.use_multithread_when_possible = false;

        // photon propagation
        ml::Photons::propagate_photons_in_scenery_with_settings(
            &photons, &optical_table, &settings, &prng);

        // detect photons in sensors
        sensor_list.clear_history();
        sensor_list.assign_photons(&photons);
        sigma_psf_vs_image_sensor_distance.push_back(
            ml::sensor::point_spread_std_dev(sensor.photon_arrival_history));
        image_sensor_distances.push_back(image_sensor_disc_distance);

        // FreeOrbitCamera free(&optical_table, &settings);
    }

    double min_sigma_psf = *std::min_element(
        sigma_psf_vs_image_sensor_distance.begin(),
        sigma_psf_vs_image_sensor_distance.end());

    unsigned int min_sigma_psf_pos = std::min_element(
            sigma_psf_vs_image_sensor_distance.begin(),
            sigma_psf_vs_image_sensor_distance.end()) -
        sigma_psf_vs_image_sensor_distance.begin();

    // sigma_psf_vs_image_sensor_distance
    // image_sensor_distances
    CHECK(0.0 == Approx(min_sigma_psf).margin(1e-3));
    CHECK(cfg.focal_length == Approx(image_sensor_distances.at(min_sigma_psf_pos)).margin(cfg.focal_length*1e-4));
    // std::cout << "smallest psf sigma of "<<min_sigma_psf*1e3<<"mm at d=";
    // std::cout << image_sensor_distances.at(min_sigma_psf_pos)*1e3<<"mm\n";
}
