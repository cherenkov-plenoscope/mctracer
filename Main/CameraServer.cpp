// Copyright 2018 Sebastian A. Mueller
#include <stdint.h>
#include "DocOpt/docopt.h"
#include "Core/Vec3.h"
#include "Visual/FlyingCamera.h"
#include "Visual/PortablePixMap.h"
#include "Scenery/Scenery.h"
#include "Tools/PathTools.h"
#include "Tools/binary_input_output.h"
#include "json.h"
using std::string;
using std::cout;

static const char USAGE[] =
R"(Show a scenery

    Usage:
      mctCameraServer --scenery=SCENERY_PATH [--config=CONFIG_PATH]
      mctCameraServer (-h | --help)
      mctCameraServer --version

    Options:
      -s --scenery=SCENERY_PATH     Scenery file path.
      -c --config=CONFIG_PATH       Visual config file path.
      -h --help                     Show this screen.
      --version                     Show version.
)";

const uint64_t MAGIC_INSTRUCTION_SYNC = 645;

struct ApertureCameraInstructions {
    uint64_t magic_sync;
    Vec3 position;
    Rot3 orientation;
    double object_distance;
    double sensor_size_along_columns;
    double field_of_view_along_columns;
    double focal_length_over_aperture_diameter;
    uint64_t number_columns;
    uint64_t number_rows;
    uint64_t noise_level;
};

ApertureCameraInstructions read_from_stream(std::istream &fin) {
    ApertureCameraInstructions inst;
    inst.magic_sync = bio::read_uint64(fin);
    inst.position = Vec3(
        bio::read_float64(fin),
        bio::read_float64(fin),
        bio::read_float64(fin));
    inst.orientation = Rot3(
        bio::read_float64(fin),
        bio::read_float64(fin),
        bio::read_float64(fin));
    inst.object_distance = bio::read_float64(fin);
    inst.sensor_size_along_columns = bio::read_float64(fin);
    inst.field_of_view_along_columns = bio::read_float64(fin);
    inst.focal_length_over_aperture_diameter = bio::read_float64(fin);
    inst.number_columns = bio::read_uint64(fin);
    inst.number_rows = bio::read_uint64(fin);
    inst.noise_level = bio::read_uint64(fin);
    return inst;
}

int main(int argc, char* argv[]) {
    try {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,        // show help if requested
            "mctCameraServer 0.1");  // version string

        PathTools::Path scenery_path = PathTools::Path(
            args.find("--scenery")->second.asString());

        Scenery scenery;
        mct::json::append_to_frame_in_scenery(
            &scenery.root,
            &scenery,
            scenery_path.path);

        Visual::Config visual_config;
        if (args.find("--config")->second) {
            visual_config = mct::json::load_visual_config(
                args.find("--config")->second.asString());
        }
        scenery.root.init_tree_based_on_mother_child_relations();

        while (true) {
            ApertureCameraInstructions ins = read_from_stream(std::cin);
            if (ins.magic_sync != MAGIC_INSTRUCTION_SYNC)
                break;

            Visual::Image image = Visual::Image(
                ins.number_columns,
                ins.number_rows);

            Visual::ApertureCamera cam(
                "camera",
                ins.number_columns,
                ins.number_rows);
            cam.set_fStop_sesnorWidth(
                ins.focal_length_over_aperture_diameter,
                ins.sensor_size_along_columns);
            cam.set_FoV_in_rad(ins.field_of_view_along_columns);
            cam.update_position(ins.position);
            cam.update_orientation(ins.orientation);
            cam.set_focus_to(ins.object_distance);
            visual_config.snapshot.noise_level = ins.noise_level;
            cam.acquire_image(&scenery.root, &visual_config, &image);
            Visual::ppm::append_image_to_file(image, std::cout);
        }

    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
