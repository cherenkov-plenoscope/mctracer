// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_
#define MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_

#include <stdint.h>

namespace Plenoscope {
namespace Calibration {

struct Config {
    uint32_t number_of_blocks;
    uint32_t photons_per_block;
    Config();
};

}  // namespace Calibration
}  // namespace Plenoscope
#endif  // MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_
