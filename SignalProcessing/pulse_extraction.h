// Copyright 2017 Sebastian A. Mueller
#ifndef SIGNALPROCESSING_PULSE_EXTRACTION_H_
#define SIGNALPROCESSING_PULSE_EXTRACTION_H_

#include <stdint.h>
#include <vector>
#include "SignalProcessing/ElectricPulse.h"
#include "SignalProcessing/ExtractedPulse.h"

namespace SignalProcessing {

const int32_t NUMBER_TIME_SLICES = 100;

std::vector<ExtractedPulse> extract_pulses(
    const std::vector<ElectricPulse> &electric_pulses,
    const double time_slice_duration);

std::vector<std::vector<ExtractedPulse>> extract_pulses(
    const std::vector<std::vector<ElectricPulse>> &electric_pulses,
    const double time_slice_duration);

}  // namespace SignalProcessing

#endif  // SIGNALPROCESSING_PULSE_EXTRACTION_H_
