// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/light_field_sensor/Config.h"
#include "merlict/merlict.h"
#include "corsika/corsika.h"

using std::array;
using namespace merlict;

namespace plenoscope {
namespace light_field_sensor {

Config::Config() {
    expected_imaging_system_focal_length = 75.0;
    expected_imaging_system_max_aperture_radius = 25.0;
    max_FoV_diameter = deg2rad(6.5);
    pixel_FoV_hex_flat2flat = deg2rad(0.1);
    number_of_paxel_on_pixel_diagonal = 13;
    housing_overhead = 1.2;
    lens_refraction = &pmma_refraction;
    lens_absorbtion = &perfect_transparency;
    bin_reflection = SurfaceEntity::DEFAULT_REFLECTION;
}

array<float, 273> Config::get_sensor_plane2imaging_system_header()const {
    array<float, 273> header;
    for (unsigned int i = 0; i < header.size(); i++) header[i] = 0.0;

    header[  1-1] = corsika::str2float("S2IS");
    header[  2-1] = 1.0;  // simulation = 1, observation = 0

    header[ 11-1] = sensor_plane2imaging_system.get_rot_x().x;
    header[ 12-1] = sensor_plane2imaging_system.get_rot_x().y;
    header[ 13-1] = sensor_plane2imaging_system.get_rot_x().z;

    header[ 14-1] = sensor_plane2imaging_system.get_rot_y().x;
    header[ 15-1] = sensor_plane2imaging_system.get_rot_y().y;
    header[ 16-1] = sensor_plane2imaging_system.get_rot_y().z;

    header[ 17-1] = sensor_plane2imaging_system.get_rot_z().x;
    header[ 18-1] = sensor_plane2imaging_system.get_rot_z().y;
    header[ 19-1] = sensor_plane2imaging_system.get_rot_z().z;

    header[ 20-1] = sensor_plane2imaging_system.get_translation().x;
    header[ 21-1] = sensor_plane2imaging_system.get_translation().y;
    header[ 22-1] = sensor_plane2imaging_system.get_translation().z;

    return header;
}

}  // namespace light_field_sensor
}  // namespace plenoscope
