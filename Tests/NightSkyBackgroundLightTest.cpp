#include "gtest/gtest.h"
#include "Plenoscope/LightFieldSensor/Config.h"
#include "Plenoscope/NightSkyBackground/NightSkyBackground.h"
#include "Tools/AsciiIo.h"
class NightSkyBackgroundLightTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(NightSkyBackgroundLightTest, init) {

    // SET UP TELESCOPE

    Plenoscope::LightFieldSensor::Config config;

    config.expected_imaging_system_focal_length = 75.0;
    config.expected_imaging_system_max_aperture_radius = 25.0;
    config.max_FoV_diameter = Deg2Rad(6.5);
    config.pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
    config.number_of_paxel_on_pixel_diagonal = 13;
    config.housing_overhead = 1.2;
    config.lens_refraction = &Plenoscope::LightFieldSensor::pmma_refraction;

    Plenoscope::LightFieldSensor::Geometry geometry(config);

    Function::LinInterpol nsb_flux_vs_wavelength(
    	AsciiIo::gen_table_from_file(
    		"night_sky_background_flux_vs_wavelength_la_palma.txt"
    	)
    );

    Plenoscope::NightSkyBackground::Light nsb(
    	&geometry,
    	&nsb_flux_vs_wavelength
    );

    EXPECT_EQ(
        nsb.fov_radius, 
        0.5*Deg2Rad(6.5)*Plenoscope::NightSkyBackground::FOV_RADIUS_OVERHEAD);

    EXPECT_EQ(
        nsb.max_principal_aperture_radius_to_trow_photons_in, 
        25*Plenoscope::NightSkyBackground::APERTURE_RADIUS_OVERHEAD);


    std::cout << nsb.__repr__() << "\n";
}