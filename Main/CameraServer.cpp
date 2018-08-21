// Copyright 2018 Sebastian A. Mueller
#include <stdint.h>
#include "DocOpt/docopt.h"
#include "Core/Photons.h"
#include "Core/Vec3.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Xml/Factory/VisualConfigFab.h"
#include "Xml/Factory/PropagationConfigFab.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Visual/FlyingCamera.h"
#include "Visual/PortablePixMap.h"
#include "Scenery/Scenery.h"
using std::string;
using std::cout;

static const char USAGE[] =
R"(Show a scenery

    Usage:
      mctCameraServer --scenery=SCENERY_PATH [--config=CONFIG_PATH]
      mctCameraServer (-h | --help)
      mctCameraServer --version

    Options:
      -s --scenery=SCENERY_PATH     Scenery xml file path.
      -c --config=CONFIG_PATH       Visual config xml file path.
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

double read_float64(std::istream &fin) {
    double v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

uint64_t read_uint64(std::istream &fin) {
    uint64_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

ApertureCameraInstructions read_from_stream(std::istream &fin) {
    ApertureCameraInstructions inst;
    inst.magic_sync = read_uint64(fin);
    inst.position = Vec3(
        read_float64(fin),
        read_float64(fin),
        read_float64(fin));
    inst.orientation = Rot3(
        read_float64(fin),
        read_float64(fin),
        read_float64(fin));
    inst.object_distance = read_float64(fin);
    inst.sensor_size_along_columns = read_float64(fin);
    inst.field_of_view_along_columns = read_float64(fin);
    inst.focal_length_over_aperture_diameter = read_float64(fin);
    inst.number_columns = read_uint64(fin);
    inst.number_rows = read_uint64(fin);
    inst.noise_level = read_uint64(fin);
    return inst;
}

int main(int argc, char* argv[]) {
    try {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,        // show help if requested
            "mctCameraServer 0.0");  // version string

        PathTools::Path scenery_path = PathTools::Path(
            args.find("--scenery")->second.asString());

        Scenery scenery;
        if (
            StringTools::is_ending(scenery_path.path, ".stl") ||
            StringTools::is_ending(scenery_path.path, ".STL")
        ) {
            const double scale = 1.0;
            StereoLitography::add_stl_to_frame(
                scenery_path.path, &scenery.root, scale);
        } else if (
            StringTools::is_ending(scenery_path.path, ".xml") ||
            StringTools::is_ending(scenery_path.path, ".XML")
        ) {
            Xml::SceneryFactory fab(scenery_path.path);
            fab.append_to_frame_in_scenery(&scenery.root, &scenery);
        } else {
            cout << "Can only read stl or xml files.\n";
            return 0;
        }

        Visual::Config visual_config;
        if (args.find("--config")->second) {
            Xml::Document doc(args.find("--config")->second.asString());
            Xml::Node node = doc.node();
            Xml::Node vc_node = node.child("visual");
            visual_config = Xml::Configs::get_VisualConfig_from_node(vc_node);
        }
        scenery.root.init_tree_based_on_mother_child_relations();

        while (true) {
            ApertureCameraInstructions ins = read_from_stream(std::cin);
            if (ins.magic_sync != MAGIC_INSTRUCTION_SYNC)
                break;

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
            cam.acquire_image(&scenery.root, &visual_config);
            const Visual::Image *camera_image = cam.get_image();

            Visual::ppm::append_image_to_file(
                *camera_image,
                std::cout);
        }

    }catch(std::exception &error) {
        std::cerr << error.what();
    }
    return 0;
}
