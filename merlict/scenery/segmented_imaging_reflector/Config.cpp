// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/segmented_imaging_reflector/Config.h"

namespace merlict {
namespace segmented_imaging_reflector {

Config::Config():
    // Default is HEGRA CT3 aka FACT
    focal_length(4.889),
    DaviesCotton_over_parabolic_mixing_factor(0.5),
    max_outer_aperture_radius(2.1),
    min_inner_aperture_radius(0.2),
    outer_aperture_shape_hex(0),
    facet_inner_hex_radius(0.30),
    gap_between_facets(0.01)
{}

}  // namespace segmented_imaging_reflector
}  // namespace merlict
