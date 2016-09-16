#include "DocOpt/docopt.h"
#include "Tools/FileTools.h"
#include "Tools/PathTools.h"
#include "Xml/Xml.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Plenoscope/Calibration/Calibrator.h"
#include "Tools/HeaderBlock.h"
#include "Scenery/Scenery.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using std::string;
using std::cout;

static const char USAGE[] =
R"(Plenoscope light field calibration

    Usage:
      mctPlenoscopeCalibration -s=SCENERY_PATH -n=NUMBER_MEGA_PHOTONS -o=OUTPUT_PATH
      mctPlenoscopeCalibration (-h | --help)
      mctPlenoscopeCalibration --version

    Options:
      -s --scenery=SCENERY_PATH                     Scenery directory path.
      -n --number_mega_photons=NUMBER_MEGA_PHOTONS  Number count [Mega Photons] 
                                                    of calibration photons to be 
                                                    emitted.
      -o --output=OUTPUT_PATH                       Output path.
      -h --help                                     Show this screen.
      --version                                     Show version.
      
)";

int main(int argc, char* argv[]) {
    try{

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0"
    );  // version string

    // parse number_mega_photons
    int number_mega_photons = 0;
    try{
        number_mega_photons = StringTools::to_int(args.find("--number_mega_photons")->second.asString());
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

    PathTools::Path out_path = PathTools::Path(args.find("--output")->second.asString());
    PathTools::Path scenery_path = PathTools::Path(args.find("--scenery")->second.asString());

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
    Scenery scenery;
    scenery_factory.append_to_frame_in_scenery(&scenery.root, &scenery);
    scenery.root.init_tree_based_on_mother_child_relations();

    if(scenery_factory.plenoscopes.size() == 0)
        throw TracerException("There is no plenoscope in the scenery");
    else if(scenery_factory.plenoscopes.size() > 1)
        throw TracerException("There is more then one plenoscope in the scenery");
    Plenoscope::PlenoscopeInScenery* pis = &scenery_factory.plenoscopes.at(0);

    HeaderBlock::write(
        pis->light_field_sensor_geometry.get_info_header(), 
        PathTools::join(out_path.path, "light_field_sensor_geometry.header.bin")
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
        &scenery.root
    );

    // WRITE OUTPUT
    calibrator.write_lixel_statistics(PathTools::join(out_path.path, "lixel_statistics.bin"));

    }catch(std::exception &error) {
        std::cerr << error.what(); 
    }
    return 0;
}