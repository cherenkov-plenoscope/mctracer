// Copyright 2014 Sebastian A. Mueller
#ifndef SIGNALPROCESSING_ELECTRICPULSE_H_
#define SIGNALPROCESSING_ELECTRICPULSE_H_

#include <stdint.h>

namespace signal_processing {

struct ElectricPulse {
    double arrival_time;
    int32_t simulation_truth_id;
    ElectricPulse();
    ElectricPulse(
        const double arrival_time,
        const int32_t simulation_truth_id);
};

}  // namespace signal_processing

#endif  // SIGNALPROCESSING_ELECTRICPULSE_H_
