#include "CommandLine/CommandLine.h"
#include "Tools/FileTools.h"
#include "Plenoscope/Plenoscope.h"
#include "Tools/PathTools.h"
#include "Xml/Xml.h"
using std::string;
using std::cout;

string help_text() {
    std::stringstream out; 
    out << "  Plenoscope calibration\n";
    out << "  --config, -c      config path steering the calibration and the plenoscope\n";
    out << "  --output, -o      output path of plenoscope calibration\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("config", 'c' ,"plenoscope calibration config path");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope calibration");
    cmd.parse(argc, argv);

    if(!cmd.exist("config") || !cmd.exist("output")) {
        cout << help_text();
        return 0;
    }

    PathTools::Path config_path = PathTools::Path(cmd.get("config"));

    cout << "out    '" << cmd.get("output") << "'\n";
    cout << "config '" << config_path.path << "'\n";

    Xml::Document doc(config_path.path);
    Xml::Node calibration = doc.node().child("calibration");
    Xml::Node block_node = calibration.child("photon_blocks");

    // SET UP TELESCOPE
    Plenoscope::Config telescope_config;
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
    telescope_config.lens_refraction = &Plenoscope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 13;
    telescope_config.object_distance_to_focus_on = 10.0e3;
    
    // INIT GEOMETRY
    Plenoscope::Geometry telescope_geometry(telescope_config);
    telescope_geometry.write_sub_pixel_positions(cmd.get("output")+"/"+"sub_pixel_positions.txt");
    FileTools::write_text_to_file(telescope_geometry.get_print(), cmd.get("output")+"/"+"overview.txt");

    // RUN LIGHT FIELD CALIBRATION
    Plenoscope::CalibrationConfig calib_config;
    calib_config.number_of_blocks = block_node.attribute2int("number_of_blocks");
    calib_config.photons_per_block = int(block_node.attribute2double("photons_per_block"));
    calib_config.raw_calibration_output_path = cmd.get("output")+"/"+"the_big_lebowsky.lftc";
    calib_config.condensed_calibration_output_path = cmd.get("output")+"/"+"sub_pixel_statistics.txt";

    Plenoscope::Calibration calib(&telescope_geometry, &calib_config);

    // RUN HIGH INCIDENT ANGLE CALIBRATION ON LENS
    //Plenoscope::LensCalibration lenscalib(&telescope_geometry);

    // EXPLORE TELESCOPE
    //Plenoscope::Factory fab(&telescope_geometry);
    //Frame telescope("telescope", Vec3::null, Rot3::null);
    //fab.add_telescope_to_frame(&telescope);

    //std::cout << fab.geometry->get_print() << "\n";

    //TracerSettings settings;
    //settings.visual.global_illumination.incoming_direction = Vec3(1.0, 1.0, 1.0);
    //FlyingCamera free(&telescope, &settings);
    
    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}