#include "CommandLine/CommandLine.h"
#include "Tools/FileTools.h"
#include "Tools/PathTools.h"
#include "Xml/Xml.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Plenoscope/Calibration/Calibrator.h"
#include "Tools/HeaderBlock.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using std::string;
using std::cout;

string help_text() {
    std::stringstream out; 
    out << "  Plenoscope calibration\n";
    out << "\n";
    out << "  --scenery, -s                 scenery with at least one plenoscope in it\n";
    out << "\n";
    out << "  --number_mega_photons, -n     the number count [Mega Photons] of\n";
    out << "                                calibration photons to be emitted\n";
    out << "\n";
    out << "  --output, -o                  output path of plenoscope calibration\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("number_mega_photons", 'n' ,"the number count [Mega Photons] of calibration photons to be emitted");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope calibration");
    cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery with at least one plenoscope in it");
    cmd.parse(argc, argv);

    if(!cmd.exist("number_mega_photons") || !cmd.exist("output") || !cmd.exist("scenery")) {
        cout << help_text();
        return 1;
    }

    // parse number_mega_photons
    int number_mega_photons = 0;
    try{
        number_mega_photons = StringTools::to_int(cmd.get("number_mega_photons"));
    }catch(StringTools::CanNotParseInt &error) {
        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Can not parse input argument '--number_mega_photons', '-n' to a integer number.";
        info << StringTools::place_first_infront_of_each_new_line_of_second("|  ", error.what());
        throw TracerException(info.str());        
    }

    if(number_mega_photons <= 0.0) {
        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected '--number_mega_photons', '-n' to be >= 0, but actual: " << number_mega_photons;
        throw TracerException(info.str());         
    }

    PathTools::Path out_path = PathTools::Path(cmd.get("output"));
    PathTools::Path scenery_path = PathTools::Path(cmd.get("scenery"));

    // 1) create output directory
    fs::create_directory(out_path.path);
    // 2) copy input into output directory
    PathTools::Path input_copy_path = PathTools::join(out_path.path, "input");
    fs::create_directory(input_copy_path.path);
    fs::copy(scenery_path.path, PathTools::join(input_copy_path.path, "scenery"));
    // 3) use the copied input files
    scenery_path = PathTools::join(input_copy_path.path, "scenery");
    PathTools::Path scenery_xml_path = PathTools::join(scenery_path.path, "scenery.xml");

    // SET UP SCENERY
    Xml::SceneryFactory scenery_factory(scenery_xml_path.path);
    Frame scenery;
    scenery.set_name_pos_rot("root", Vec3::null, Rot3::null);
    scenery_factory.add_scenery_to_frame(&scenery);
    scenery.init_tree_based_on_mother_child_relations();

    if(scenery_factory.plenoscopes.size() == 0)
        throw TracerException("There is no plenoscope in the scenery");
    else if(scenery_factory.plenoscopes.size() > 1)
        throw TracerException("There is more then one plenoscope in the scenery");
    Plenoscope::PlenoscopeInScenery* pis = &scenery_factory.plenoscopes.at(0);

    HeaderBlock::write(
        pis->light_field_sensor_geometry.get_info_header(), 
        PathTools::join(cmd.get("output"), "light_field_sensor_geometry.header.bin")
    );

    pis->light_field_sensor_geometry.write_lixel_positions(
        PathTools::join(out_path.path, "lixel_positions.bin")
    );

    // CALIBRATION CONFIG
    Plenoscope::Calibration::Config calib_config;
    calib_config.number_of_blocks = number_mega_photons;
    calib_config.photons_per_block = int(1e6);

    // RUN PLENOSCOPE CALIBRATION   
    Plenoscope::Calibration::Calibrator calibrator(
        calib_config, 
        pis,
        &scenery
    );

    // WRITE OUTPUT
    calibrator.write_lixel_statistics(PathTools::join(cmd.get("output"), "lixel_statistics.bin"));

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}