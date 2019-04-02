// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_
#define MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_

#include "merlict/Color.h"
#include "merlict/function/function.h"

namespace merlict {
namespace segmented_imaging_reflector {

struct Config {
    double focal_length;
    double DaviesCotton_over_parabolic_mixing_factor;
    double max_outer_aperture_radius;
    double min_inner_aperture_radius;
    double facet_inner_hex_radius;
    double gap_between_facets;
    Config();
};

}  // namespace segmented_imaging_reflector
}  // namespace merlict

#endif  // MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_
