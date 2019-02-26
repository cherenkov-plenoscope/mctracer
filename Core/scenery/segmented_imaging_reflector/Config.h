// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_
#define CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_

#include "Core/Color.h"
#include "Core/function/function.h"

namespace relleums {
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
}  // namespace relleums

#endif  // CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_CONFIG_H_
