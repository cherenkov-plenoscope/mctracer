#include "gtest/gtest.h"
#include "LightFieldTelescope/NightSkyBackgroundLight.h"

class NightSkyBackgroundLightTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(NightSkyBackgroundLightTest, init) {

    // SET UP TELESCOPE
    LightFieldTelescope::Config telescope_config;

    telescope_config.reflector.focal_length = 75.0;
    telescope_config.reflector.DaviesCotton_over_parabolic_mixing_factor = 0.0;
    telescope_config.reflector.max_outer_aperture_radius = 25.0;
    telescope_config.reflector.min_inner_aperture_radius = 0.5;
    telescope_config.reflector.facet_inner_hex_radius = 0.6;
    telescope_config.reflector.gap_between_facets = 0.01;
    telescope_config.reflector.reflectivity = &SegmentedReflector::perfect_reflectivity;
    telescope_config.max_FoV_diameter = Deg2Rad(6.5);
    telescope_config.pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
    telescope_config.housing_overhead = 1.2;
    telescope_config.lens_refraction = &LightFieldTelescope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 11;	


    LightFieldTelescope::Geometry telescope_geometry(telescope_config);

    Function::LinInterpol nsb_flux_vs_wavelength(
    	AsciiIo::gen_table_from_file(
    		"night_sky_background_flux_vs_wavelength_la_palma.txt"
    	)
    );

    LightFieldTelescope::NightSkyBackgroundLight nsb(
    	&telescope_geometry,
    	&nsb_flux_vs_wavelength
    );

    std::cout << nsb << "\n";
    Random::Mt19937 prng(0);

    //std::vector<Photon*>* nsb_photons = 
    //	nsb.get_photons_in_duration(0.0, 1e-9, &prng);

    //std::cout << Photons::get_print(nsb_photons);
}