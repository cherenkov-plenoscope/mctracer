// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_SIMULATIONTRUTHHEADER_H_
#define PLENOSCOPE_SIMULATIONTRUTHHEADER_H_

#include <stdint.h>
#include <array>
#include "LightFieldSensor/Config.h"

namespace Plenoscope {

struct SimulationTruthHeader {
    std::array<float, 273> raw;
    SimulationTruthHeader();
    void set_random_number_seed_of_run(const uint32_t seed);
};

}
#endif  // PLENOSCOPE_SIMULATIONTRUTHHEADER_H_
