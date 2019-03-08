// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/EventHeader.h"
#include "corsika/corsika.h"

namespace plenoscope {

EventHeader::EventHeader() {
    for (unsigned int i = 0; i < raw.size(); i++) raw[i]=0.0;
    raw[  1-1] = corsika::str2float("PEVT");
}

void EventHeader::set_event_type(const float event_type) {
    raw[  2-1] = event_type;
}

void EventHeader::set_trigger_type(const float trigger_type) {
    raw[  3-1] = trigger_type;
}

void EventHeader::set_plenoscope_geometry(const light_field_sensor::Config &cfg) {
    raw[ 11-1] = cfg.sensor_plane2imaging_system.rot_x().x;
    raw[ 12-1] = cfg.sensor_plane2imaging_system.rot_x().y;
    raw[ 13-1] = cfg.sensor_plane2imaging_system.rot_x().z;

    raw[ 14-1] = cfg.sensor_plane2imaging_system.rot_y().x;
    raw[ 15-1] = cfg.sensor_plane2imaging_system.rot_y().y;
    raw[ 16-1] = cfg.sensor_plane2imaging_system.rot_y().z;

    raw[ 17-1] = cfg.sensor_plane2imaging_system.rot_z().x;
    raw[ 18-1] = cfg.sensor_plane2imaging_system.rot_z().y;
    raw[ 19-1] = cfg.sensor_plane2imaging_system.rot_z().z;

    raw[ 20-1] = cfg.sensor_plane2imaging_system.translation().x;
    raw[ 21-1] = cfg.sensor_plane2imaging_system.translation().y;
    raw[ 22-1] = cfg.sensor_plane2imaging_system.translation().z;

    raw[ 23-1] = cfg.expected_imaging_system_focal_length;
    raw[ 24-1] = cfg.expected_imaging_system_max_aperture_radius;
    raw[ 25-1] = cfg.max_FoV_diameter;
    raw[ 26-1] = cfg.pixel_FoV_hex_flat2flat;
    raw[ 27-1] = cfg.num_paxel_on_pixel_diagonal;
    raw[ 28-1] = cfg.housing_overhead;
}

}  // namespace plenoscope
