// Copyright 2014 Sebastian A. Mueller
#include "merlict_portal_plenoscope/calibration/Config.h"

namespace plenoscope {
namespace calibration {

Config::Config() {
    num_blocks = 8;
    photons_per_block = 1e6;
}

}  // namespace calibration
}  // namespace plenoscope
