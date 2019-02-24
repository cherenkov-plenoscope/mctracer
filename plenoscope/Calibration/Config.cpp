// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/Calibration/Config.h"

namespace plenoscope {
namespace Calibration {

Config::Config() {
    number_of_blocks = 8;
    photons_per_block = 1e6;
}

}  // namespace Calibration
}  // namespace plenoscope
