// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CONFIG_H_
#define VISUAL_CONFIG_H_

#include "merlict/Color.h"
#include "SkyDome.h"
#include "merlict/Vec3.h"

namespace merlict {
namespace visual {

struct Config {
    unsigned int max_interaction_depth;

    struct Preview {
        unsigned int cols;
        unsigned int rows;
        unsigned int scale;
    } preview;

    struct Snapshot {
        unsigned int cols;
        unsigned int rows;
        unsigned int noise_level;
        double focal_length_over_aperture_diameter;
        double image_sensor_size_along_a_row;
    } snapshot;

    struct GlobalIllumination {
        bool on;
        Vec3 incoming_direction;
    } global_illumination;

    struct PhotonTrajectories {
        double radius;
    } photon_trajectories;
    SkyDome sky_dome;
    Config();
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_CONFIG_H_
