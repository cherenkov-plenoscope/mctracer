#include "Cameras/FlyingCamera.h"
#include "Tools/FileTools.h"
#include "LightFieldTelescope/LightFieldTelescope.h"

int main(int argc, char* argv[]) {
    try{

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
    telescope_config.pixel_FoV_hex_flat2flat = Deg2Rad(0.0667);
    telescope_config.housing_overhead = 1.2;
    telescope_config.lens_refraction = &LightFieldTelescope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 7;
    telescope_config.object_distance_to_focus_on = 10.0e3;
    
    // INIT GEOMETRY
    LightFieldTelescope::Geometry telescope_geometry(telescope_config);
    telescope_geometry.write_sub_pixel_positions("sub_pixel_positions.csv");
    FileTools::write_text_to_file(
        telescope_geometry.get_print(),"overview.txt"
    );

    // RUN HIGH INCIDENT ANGLE CALIBRATION ON LENS
    LightFieldTelescope::LensCalibration lenscalib(&telescope_geometry);

    // RUN LIGHT FIELD CALIBRATION
    LightFieldTelescope::CalibrationConfig calib_config;
    calib_config.photons_per_sub_pixel = 25;
    calib_config.photons_per_block = 1e6;

    LightFieldTelescope::Calibration calib(&telescope_geometry, &calib_config);
    //calib.export_sub_pixel_statistics("sub_pixel_statistics.txt");

    // EXPLORE TELESCOPE
    LightFieldTelescope::Factory fab(&telescope_geometry);
    Frame telescope("telescope", Vector3D::null, Rotation3D::null);
    fab.add_telescope_to_frame(&telescope);

    std::cout << fab.geometry->get_print() << "\n";

    TracerSettings settings;
    settings.global_light_direction = Vector3D(1.0, 1.0, 1.0);
    FlyingCamera free(&telescope, &settings);
    
    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}