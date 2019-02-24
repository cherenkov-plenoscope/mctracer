// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/SimulationTruthHeader.h"
#include <sstream>
#include "corsika/corsika.h"

namespace plenoscope {

SimulationTruthHeader::SimulationTruthHeader() {
    for (unsigned int i = 0; i < raw.size(); i++) raw[i]=0.0;
    raw[  1-1] = Corsika::str2float("MTST");
}

void SimulationTruthHeader::set_random_number_seed_of_run(const uint32_t seed) {
    float f;
    memcpy(&f, &seed, sizeof f);
    raw[  1-1] = f;
}

}  // namespace plenoscope
