// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_
#define MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_

#include <stdint.h>

namespace plenoscope {
namespace calibration {

struct Config {
    uint32_t num_blocks;
    uint32_t photons_per_block;
    Config();
};

}  // namespace calibration
}  // namespace plenoscope

#endif  // MCTRACER_PLENOSCOPE_CALIBRATION_CONFIG_H_
