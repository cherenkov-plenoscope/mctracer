// Copyright 2014 Sebastian A. Mueller
#include "Visual/Config.h"

namespace visual {

Config::Config():sky_dome(Color::SKY_BLUE) {
    max_interaction_depth = 42;

    preview.cols = 128;
    preview.rows = 72;
    preview.scale = 10;

    snapshot.cols = 1920;
    snapshot.rows = 1080;
    snapshot.noise_level = 16;  // fairly smooth
    snapshot.focal_length_over_aperture_diameter = 0.95;  // Ultra fast lens
    snapshot.image_sensor_size_along_a_row = 0.07;  // IMAX 70mm format

    global_illumination.on = true;
    global_illumination.incoming_direction = Vec3(0.6, 0.2, 1.0);

    photon_trajectories.radius = 0.025;
}

}  // namespace visual
