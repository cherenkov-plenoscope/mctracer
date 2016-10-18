//=================================
// include guard
#ifndef __PlenoscopeSimulationTruthHeader_H_INCLUDED__
#define __PlenoscopeSimulationTruthHeader_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldSensor/Config.h"
#include <stdint.h>

namespace Plenoscope {
    
    struct SimulationTruthHeader {
        array<float, 273> raw;
        SimulationTruthHeader();
        void set_random_number_seed_of_run(const uint32_t seed);
    };
}
#endif // __PlenoscopeSimulationTruthHeader_H_INCLUDED__ 