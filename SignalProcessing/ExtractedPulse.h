// Copyright 2017 Sebastian A. Mueller
#ifndef SIGNALPROCESSING_EXTRACTEDPULSE_H_
#define SIGNALPROCESSING_EXTRACTEDPULSE_H_

#include <stdint.h>

namespace SignalProcessing {

struct ExtractedPulse {
    uint8_t arrival_time_slice;
    int32_t simulation_truth_id;
    ExtractedPulse();
    ExtractedPulse(
        const uint8_t arrival_time_slice,
        const int32_t simulation_truth_id);
};

}  // namespace SignalProcessing

#endif  // SIGNALPROCESSING_EXTRACTEDPULSE_H_
