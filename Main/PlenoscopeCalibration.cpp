#include "CommandLine/CommandLine.h"
#include "Tools/FileTools.h"
#include "Tools/PathTools.h"
#include "Xml/Xml.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Plenoscope/Calibration/Calibrator.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using std::string;
using std::cout;

string help_text() {
    std::stringstream out; 
    out << "  Plenoscope calibration\n";
    out << "  --scenery, -s     scenery with at least one plenoscope in it\n";
    out << "  --config, -c      config path steering the calibration and the plenoscope\n";
    out << "  --output, -o      output path of plenoscope calibration\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("config", 'c' ,"plenoscope calibration config path");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope calibration");
    cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery with at least one plenoscope in it");
    cmd.parse(argc, argv);

    if(!cmd.exist("config") || !cmd.exist("output") || !cmd.exist("scenery")) {
        cout << help_text();
        return 1;
    }

    PathTools::Path config_path = PathTools::Path(cmd.get("config"));
    PathTools::Path out_path = PathTools::Path(cmd.get("output"));
    PathTools::Path scenery_path = PathTools::Path(cmd.get("scenery"));

    // 1) create output directory
    fs::create_directory(out_path.path);
    // 2) copy input into output directory
    PathTools::Path input_copy_path = PathTools::join(out_path.path, "input");
    fs::create_directory(input_copy_path.path);
    fs::copy(config_path.path, PathTools::join(input_copy_path.path, "calibration_config.xml"));
    fs::copy(scenery_path.path, PathTools::join(input_copy_path.path, "scenery.xml"));
    // 3) use the copied input files
    config_path = PathTools::join(input_copy_path.path, "calibration_config.xml");
    scenery_path = PathTools::join(input_copy_path.path, "scenery.xml");

    Xml::Document doc(config_path.path);
    Xml::Node calibration = doc.node().child("calibration");
    Xml::Node block_node = calibration.child("photon_blocks");

    // SET UP SCENERY
    Xml::SceneryFactory scenery_factory(scenery_path.path);
    Frame scenery("root", Vec3::null, Rot3::null);
    scenery_factory.add_scenery_to_frame(&scenery);
    scenery.init_tree_based_on_mother_child_relations();

    if(scenery_factory.plenoscopes.size() == 0)
        throw TracerException("There is no plenoscope in the scenery");
    else if(scenery_factory.plenoscopes.size() > 1)
        throw TracerException("There is more then one plenoscope in the scenery");
    Plenoscope::PlenoscopeInScenery* pis = &scenery_factory.plenoscopes.at(0);

    pis->light_field_sensor_geometry.write_lixel_positions(
        PathTools::join(out_path.path, "lixel_positions.txt")
    );

    // CALIBRATION CONFIG
    Plenoscope::Calibration::Config calib_config;
    calib_config.number_of_blocks = block_node.attribute2int("number_of_blocks");
    calib_config.photons_per_block = int(block_node.attribute2double("photons_per_block"));

    // RUN PLENOSCOPE CALIBRATION   
    Plenoscope::Calibration::Calibrator calibrator(
        calib_config, 
        pis,
        &scenery
    );
    calibrator.write_lixel_statistics(PathTools::join(cmd.get("output"), "lixel_statistics.bin"));
    calibrator.write_lixel_statistics_header(PathTools::join(cmd.get("output"), "lixel_statistics.header.bin"));

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}