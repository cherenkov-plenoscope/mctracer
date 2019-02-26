// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "plenoscope/light_field_sensor/Config.h"
#include "plenoscope/night_sky_background/NightSkyBackground.h"
#include "merlict/mctracer.h"
#include "Tools/AsciiIo.h"

using namespace relleums;



TEST_CASE("NightSkyBackgroundLightTest: init", "[mctracer]") {
    // SET UP TELESCOPE
    plenoscope::light_field_sensor::Config config;

    config.expected_imaging_system_focal_length = 75.0;
    config.expected_imaging_system_max_aperture_radius = 25.0;
    config.max_FoV_diameter = deg2rad(6.5);
    config.pixel_FoV_hex_flat2flat = deg2rad(0.1);
    config.number_of_paxel_on_pixel_diagonal = 13;
    config.housing_overhead = 1.2;
    config.lens_refraction = &plenoscope::light_field_sensor::pmma_refraction;

    plenoscope::light_field_sensor::Geometry geometry(config);

    function::Func1 nsb_flux_vs_wavelength(
        tsvio::gen_table_from_file(
            "night_sky_background_flux_vs_wavelength_la_palma.txt"));

    plenoscope::night_sky_background::Light nsb(
        &geometry,
        &nsb_flux_vs_wavelength);

    CHECK(0.5*deg2rad(6.5)*plenoscope::night_sky_background::FOV_RADIUS_OVERHEAD == nsb.fov_radius);
    CHECK(25*plenoscope::night_sky_background::APERTURE_RADIUS_OVERHEAD == nsb.aperture_radius);

    std::cout << nsb.str() << "\n";
}
