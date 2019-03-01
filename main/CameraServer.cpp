// Copyright 2018 Sebastian A. Mueller
#include <stdint.h>
#include <iostream>
#include "docopt/docopt.h"
#include "merlict/merlict.h"
#include "./json.h"
namespace ml = merlict;


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
    ml::Vec3 position;
    ml::Rot3 orientation;
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
    inst.magic_sync = ml::binio::read_uint64(fin);
    double x, y, z;
    x = ml::binio::read_float64(fin);
    y = ml::binio::read_float64(fin);
    z = ml::binio::read_float64(fin);
    inst.position = ml::Vec3(x, y, z);
    x = ml::binio::read_float64(fin);
    y = ml::binio::read_float64(fin);
    z = ml::binio::read_float64(fin);
    inst.orientation = ml::Rot3(x, y, z);
    inst.object_distance = ml::binio::read_float64(fin);
    inst.sensor_size_along_columns = ml::binio::read_float64(fin);
    inst.field_of_view_along_columns = ml::binio::read_float64(fin);
    inst.focal_length_over_aperture_diameter = ml::binio::read_float64(fin);
    inst.number_columns = ml::binio::read_uint64(fin);
    inst.number_rows = ml::binio::read_uint64(fin);
    inst.noise_level = ml::binio::read_uint64(fin);
    return inst;
}

int main(int argc, char* argv[]) {
    try {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,        // show help if requested
            "mctCameraServer 0.1");  // version string

        ml::ospath::Path scenery_path(
            args.find("--scenery")->second.asString());

        ml::Scenery scenery;
        ml::json::append_to_frame_in_scenery(
            &scenery.root,
            &scenery,
            scenery_path.path);

        ml::visual::Config visual_config;
        if (args.find("--config")->second) {
            visual_config = ml::json::load_visual_config(
                args.find("--config")->second.asString());
        }
        scenery.root.init_tree_based_on_mother_child_relations();

        while (true) {
            ApertureCameraInstructions ins = read_from_stream(std::cin);
            if (ins.magic_sync != MAGIC_INSTRUCTION_SYNC)
                break;

            ml::visual::Image image(
                ins.number_columns,
                ins.number_rows);

            ml::visual::ApertureCamera cam(
                "camera",
                ins.number_columns,
                ins.number_rows);
            cam.set_fStop_sesnorWidth(
                ins.focal_length_over_aperture_diameter,
                ins.sensor_size_along_columns);
            cam.set_FoV_in_rad(ins.field_of_view_along_columns);
            cam.update_position_and_orientation(
                ins.position,
                ins.orientation);
            cam.set_focus_to(ins.object_distance);
            visual_config.snapshot.noise_level = ins.noise_level;
            cam.acquire_image(&scenery.root, &visual_config, &image);
            ml::visual::ppm::append_image_to_file(image, std::cout);
        }
    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
