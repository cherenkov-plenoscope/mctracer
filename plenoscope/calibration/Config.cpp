// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/calibration/Config.h"

namespace plenoscope {
namespace calibration {

Config::Config() {
    number_of_blocks = 8;
    photons_per_block = 1e6;
}

}  // namespace calibration
}  // namespace plenoscope
