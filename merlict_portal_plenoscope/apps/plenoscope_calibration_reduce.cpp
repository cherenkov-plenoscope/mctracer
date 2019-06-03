// Copyright 2019 Sebastian A. Mueller
#include <experimental/filesystem>
#include <iostream>
#include "docopt/docopt.h"
#include "merlict_portal_plenoscope/calibration/Calibrator.h"
#include "merlict_portal_plenoscope/calibration/LixelStatisticsFiller.h"
#include "merlict_portal_plenoscope/json_to_plenoscope.h"
#include "merlict_corsika/corsika.h"
#include "merlict/merlict.h"
namespace fs = std::experimental::filesystem;
namespace ml = merlict;


static const char USAGE[] =
R"(reduction light-field-calibration for the Portal Cherenkov-plenoscope

    Usage:
      plenoscope-calibration-reduce -i=PATH -o=PATH
      plenoscope-calibration-reduce (-h | --help)
      plenoscope-calibration-reduce --version

    Options:
      -i --input=PATH                               Input directory.
      -o --output=PATH                              Output path.
      -h --help                                     Show this screen.
      --version                                     Show version.
)";

std::vector<plenoscope::CalibrationPhotonResult> read_calib_block(
    const std::string path) {
    std::vector<plenoscope::CalibrationPhotonResult> out;
    std::ifstream fin;
    fin.open(path, std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Lightfield-Calibration: Unable to read file: '";
        info << path << "'\n";
        throw std::runtime_error(info.str());
    }
    while (fin) {
        plenoscope::CalibrationPhotonResult res;
        fin.read(
            reinterpret_cast<char*>(&res),
            sizeof(plenoscope::CalibrationPhotonResult));
        out.push_back(res);
    }
    fin.close();
    return out;
}


int main(int argc, char* argv[]) {
    try {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,    // show help if requested
            "0.0");  // version string

        ml::ospath::Path out_path(args.find("--output")->second.asString());
        ml::ospath::Path in_path(args.find("--input")->second.asString());

        std::vector<std::string> paths;
        for (auto& p : fs::directory_iterator(in_path.path)) {
            paths.push_back(p.path());
        }

        if (paths.size() < 1) {
            std::stringstream info;
            info << __FILE__ << " " << __LINE__ << "\n";
            info << "Expected at least one raw calibration-block in path: ";
            info << in_path.path << "'\n";
            throw std::runtime_error(info.str());
        }

        // 1) create output directory
        fs::create_directory(out_path.path);

        // 2) copy input into output directory
        std::string ref_path = paths.at(0);
        std::string inputs[4] = {
            "input",
            "info.md",
            "light_field_sensor_geometry.header.bin",
            "lixel_positions.bin"};
        for (std::string input : inputs) {
            fs::copy(
                ml::ospath::join(ref_path, input),
                ml::ospath::join(out_path.path, input),
                fs::copy_options::recursive);
        }

        std::array<float, 273> ref_header = corsika::read_273_f4_from_path(
            ml::ospath::join(
                out_path.path,
                "light_field_sensor_geometry.header.bin")).at(0);

        std::vector<plenoscope::CalibrationPhotonResult> ref_photons =
            read_calib_block(
                ml::ospath::join(
                    ref_path,
                    "raw_lixel_statistics.bin"));

        const uint64_t num_blocks = paths.size();
        const uint64_t num_photons_per_block = ref_photons.size();
        const uint64_t num_lixel = (
            static_cast<uint64_t>(ref_header[101 - 1])*
            static_cast<uint64_t>(ref_header[102 - 1]));

        plenoscope::calibration::LixelStatisticsFiller lixel_statistics_filler(
            num_lixel,
            num_blocks,
            num_photons_per_block);

        for (std::string p : paths) {
            std::array<float, 273> header = corsika::read_273_f4_from_path(
                ml::ospath::join(
                    p,
                    "light_field_sensor_geometry.header.bin")).at(0);

            if (!corsika::is_equal_273_f4(header, ref_header)) {
                std::stringstream info;
                info << __FILE__ << " " << __LINE__ << "\n";
                info << "Expected all headers to be equal, but header in ";
                info << p << "' is not.\n";
                throw std::runtime_error(info.str());
            }

            std::vector<plenoscope::CalibrationPhotonResult> calib_block =
                read_calib_block(
                    ml::ospath::join(
                        p,
                        "raw_lixel_statistics.bin"));

            lixel_statistics_filler.fill_in_block(calib_block);
        }

        std::vector<plenoscope::calibration::LixelStatistic> lixel_statistics =
            lixel_statistics_filler.get_lixel_statistics();

        plenoscope::calibration::write(
            lixel_statistics,
            ml::ospath::join(
                out_path.path,
                "lixel_statistics.bin"));
    } catch (std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
