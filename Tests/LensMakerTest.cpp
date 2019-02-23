// Copyright 2014 Sebastian A. Mueller
#include <algorithm>
#include <vector>
#include "gtest/gtest.h"
#include "Core/Color.h"
#include "Scenery/Primitive/BiConvexLensHexBound.h"
#include "Scenery/Primitive/Disc.h"
#include "Scenery/Geometry/LensMaker/LensMaker.h"
#include "Core/random/random.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Core/Photons.h"
using std::vector;
using namespace relleums;

class LensMakerTest : public ::testing::Test {};

TEST_F(LensMakerTest, check_sebastians_paper_and_pen_calculation) {
    double expected_curvature_radius = 0.125;;
    LensMaker::Config cfg;
    cfg.focal_length = 0.1335;
    cfg.aperture_radius = 0.071;
    cfg.refractive_index = 1.49;
    EXPECT_NEAR(
        expected_curvature_radius,
        LensMaker::Approximation::get_curvature_radius(cfg),
        expected_curvature_radius*3e-2);
}

TEST_F(LensMakerTest, lens_thicknes) {
    double expected_curvature_radius = 0.125;
    double expected_thickness = 0.0445;
    LensMaker::Config cfg;
    cfg.focal_length = 0.1335;
    cfg.aperture_radius = 0.071;
    cfg.refractive_index = 1.49;
    EXPECT_NEAR(
        expected_thickness,
        LensMaker::Approximation::get_lens_thickness_for_R_r(
            expected_curvature_radius,
            cfg.aperture_radius),
        expected_thickness*3e-2);
}

TEST_F(LensMakerTest, check_lensmaker_on_optical_table_with_lens) {
    // Hello LensMaker,
    LensMaker::Config cfg;
    // we want a lens with
    cfg.focal_length = 1.0;  // 0.1335;
    // made out of pmma plastic with
    cfg.refractive_index = 1.49;
    // and an outer aperture radius of
    cfg.aperture_radius = 0.071;
    // please tell us the needed curvature radius.

    double lens_curvature_radius =
        LensMaker::Approximation::get_curvature_radius(cfg);

    // ok lets test it...
    const Color* lens_col = &COLOR_GRAY;
    const Color* sensor_disc_col = &COLOR_DARK_GRAY;
    const Function::Func1 refraction_vs_wavelength(
        {
            {200e-9, cfg.refractive_index},
            {1200e-9, cfg.refractive_index}
        });
    random::Mt19937 prng(random::ZERO_SEED);
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
        Frame optical_table;
        optical_table.set_name_pos_rot("table", VEC3_ORIGIN, ROT3_UNITY);
        BiConvexLensHexBound* lens =
            optical_table.append<BiConvexLensHexBound>();
        lens->set_name_pos_rot("lens", VEC3_ORIGIN, ROT3_UNITY);
        lens->set_outer_color(lens_col);
        lens->set_inner_color(lens_col);
        lens->set_inner_refraction(&refraction_vs_wavelength);
        lens->set_curvature_radius_and_outer_hex_radius(
            lens_curvature_radius,
            cfg.aperture_radius);
        Disc* sensor_disc = optical_table.append<Disc>();
        sensor_disc->set_name_pos_rot(
            "sensor_disc",
            Vec3(0.0, 0.0, -image_sensor_disc_distance),
            ROT3_UNITY);
        sensor_disc->set_outer_color(sensor_disc_col);
        sensor_disc->set_inner_color(sensor_disc_col);
        sensor_disc->set_radius(cfg.aperture_radius*0.85);
        PhotonSensor::Sensor sensor(0, sensor_disc);
        vector<PhotonSensor::Sensor*> sensor_vec = {&sensor};
        PhotonSensor::Sensors sensor_list(sensor_vec);
        optical_table.init_tree_based_on_mother_child_relations();

        // light source
        vector<Photon> photons =
            Photons::Source::parallel_towards_z_from_xy_disc(
                cfg.aperture_radius*0.85,  // 0.85 inner hex radius
                number_of_photons_per_run,
                &prng);
        HomTra3 Trafo;
        Trafo.set_transformation(
            Rot3(0.0, -deg2rad(180.0), 0.0),
            Vec3(0.0, 0.0, 2.0));
        Photons::transform_all_photons(Trafo, &photons);

        // propagation settings
        PropagationConfig settings;
        settings.use_multithread_when_possible = false;

        // photon propagation
        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &optical_table, &settings, &prng);

        // detect photons in sensors
        sensor_list.clear_history();
        sensor_list.assign_photons(&photons);
        sigma_psf_vs_image_sensor_distance.push_back(
            PhotonSensor::point_spread_std_dev(sensor.photon_arrival_history));
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
    EXPECT_NEAR(0.0, min_sigma_psf, 1e-3);
    EXPECT_NEAR(
        cfg.focal_length,
        image_sensor_distances.at(min_sigma_psf_pos),
        cfg.focal_length*1e-4);
    // std::cout << "smallest psf sigma of "<<min_sigma_psf*1e3<<"mm at d=";
    // std::cout << image_sensor_distances.at(min_sigma_psf_pos)*1e3<<"mm\n";
}
