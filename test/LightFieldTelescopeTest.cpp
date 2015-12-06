#include "gtest/gtest.h"
#include "Cameras/FreeOrbitCamera.h"
#include "LightFieldTelescope/LightFieldTelescope.h"

class LightFieldTelescopeTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(LightFieldTelescopeTest, calibration) {


    LightFieldTelescope::Config telescope_config;

    telescope_config.reflector.focal_length = 75.0;
    telescope_config.reflector.DaviesCotton_over_parabolic_mixing_factor = 0.0;
    telescope_config.reflector.max_outer_aperture_radius = 25.0;
    telescope_config.reflector.min_inner_aperture_radius = 0.5;
    telescope_config.reflector.facet_inner_hex_radius = 0.6;
    telescope_config.reflector.gap_between_facets = 0.01;
    telescope_config.reflector.reflectivity = &SegmentedReflector::typical_reflectivity;
    telescope_config.max_FoV_diameter = Deg2Rad(6.5);
    telescope_config.pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
    telescope_config.housing_overhead = 1.2;
    telescope_config.lens_refraction = &LightFieldTelescope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 11;

    LightFieldTelescope::Calibration calib(telescope_config);
    //LightFieldTelescope::LensCalibration lenscalib(telescope_config);

    
    Frame telescope("telescope", Vector3D::null, Rotation3D::null);

    LightFieldTelescope::Factory fab(telescope_config);
    fab.add_telescope_to_frame(&telescope);

    std::cout << fab.geometry << "\n";

    TracerSettings settings;
    settings.global_light_direction = Vector3D::unit_z;
    FreeOrbitCamera free(&telescope, &settings);
}
