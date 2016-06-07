#include "gtest/gtest.h"
#include "Plenoscope/LightFieldSensor/Config.h"
#include "Plenoscope/NightSkyBackground/Light.h"
#include "Plenoscope/NightSkyBackground/Injector.h"
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
    config.sub_pixel_on_pixel_diagonal = 13;
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

    std::cout << nsb << "\n";
    Random::Mt19937 prng(0);

    //std::vector<Photon*>* nsb_photons = 
    //	nsb.get_photons_in_duration(0.0, 1e-9, &prng);

    //std::cout << Photons::get_print(nsb_photons);
}