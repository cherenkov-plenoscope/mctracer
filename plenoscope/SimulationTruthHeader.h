// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_SIMULATIONTRUTHHEADER_H_
#define PLENOSCOPE_SIMULATIONTRUTHHEADER_H_

#include <stdint.h>
#include <array>

namespace plenoscope {

struct SimulationTruthHeader {
    std::array<float, 273> raw;
    SimulationTruthHeader();
    void set_random_seed_of_run(const uint32_t seed);
};

}  // namespace plenoscope

#endif  // PLENOSCOPE_SIMULATIONTRUTHHEADER_H_
