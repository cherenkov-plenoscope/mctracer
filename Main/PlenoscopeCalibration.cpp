// Copyright 2015 Sebastian A. Mueller
#include <experimental/filesystem>
#include "DocOpt/docopt.h"
#include "Tools/FileTools.h"
#include "Tools/PathTools.h"
#include "Plenoscope/Calibration/Calibrator.h"
#include "Plenoscope/json_to_plenoscope.h"
#include "Tools/HeaderBlock.h"
#include "Scenery/Scenery.h"
namespace fs = std::experimental::filesystem;
using std::string;
using std::cout;
using std::stringstream;
using PathTools::Path;
using PathTools::join;

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
    try {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,        // show help if requested
            "mct 0.1");  // version string

        int number_mega_photons = 0;
        try {
            number_mega_photons = StringTools::to_int(
                args.find("--number_mega_photons")->second.asString());
        } catch (std::invalid_argument &error) {
            stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "Expected ";
            info << "'--number_mega_photons', '-n' to be an integer number, ";
            info << "but actual: " << args.find("--number_mega_photons"
                )->second.asString();
            throw std::invalid_argument(info.str());
        }

        if (number_mega_photons <= 0.0) {
            stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "Expected '--number_mega_photons', ";
            info << "'-n' to be >= 0, but actual: " << number_mega_photons;
            throw std::invalid_argument(info.str());
        }

        Path out_path = Path(args.find("--output")->second.asString());
        Path scenery_path = Path(args.find("--scenery")->second.asString());

        // 1) create output directory
        fs::create_directory(out_path.path);
        // 2) copy input into output directory
        Path input_copy_path = join(out_path.path, "input");
        fs::create_directory(input_copy_path.path);
        fs::copy(
            scenery_path.path,
            join(input_copy_path.path, "scenery"));
        // 3) use the copied input files
        scenery_path = join(input_copy_path.path, "scenery");
        Path scenery_file_path = join(scenery_path.path, "scenery.json");

        // SET UP SCENERY
        Plenoscope::PlenoscopeScenery scenery;
        Plenoscope::json::append_to_frame_in_scenery(
            &scenery.root,
            &scenery,
            scenery_file_path.path);
        scenery.root.init_tree_based_on_mother_child_relations();

        if (scenery.plenoscopes.size() == 0)
            throw std::invalid_argument(
                "There is no plenoscope in the scenery");
        else if (scenery.plenoscopes.size() > 1)
            throw std::invalid_argument(
                "There is more than one plenoscope in the scenery");
        Plenoscope::PlenoscopeInScenery* pis = &scenery.plenoscopes.at(0);

        HeaderBlock::write(
            pis->light_field_sensor_geometry.get_info_header(),
            join(out_path.path, "light_field_sensor_geometry.header.bin"));

        pis->light_field_sensor_geometry.write_lixel_positions(
            join(out_path.path, "lixel_positions.bin"));

        pis->light_field_sensor_geometry.write_info_header(
            join(out_path.path, "info.md"));

        // CALIBRATION CONFIG
        Plenoscope::Calibration::Config calib_config;
        calib_config.number_of_blocks = number_mega_photons;
        calib_config.photons_per_block = static_cast<int>(1e6);

        // RUN PLENOSCOPE CALIBRATION
        Plenoscope::Calibration::Calibrator calibrator(
            calib_config,
            pis,
            &scenery.root);

        // WRITE OUTPUT
        calibrator.write_lixel_statistics(
            join(out_path.path, "lixel_statistics.bin"));
    } catch (std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
