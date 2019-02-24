// Copyright 2017 Sebastian A. Mueller
#ifndef SIGNALPROCESSING_PULSE_EXTRACTION_H_
#define SIGNALPROCESSING_PULSE_EXTRACTION_H_

#include <stdint.h>
#include <vector>
#include "signal_processing/ElectricPulse.h"
#include "signal_processing/ExtractedPulse.h"
#include "Core/mctracer.h"

namespace signal_processing {

const int32_t NUMBER_TIME_SLICES = 100;

std::vector<ExtractedPulse> extract_pulses(
    const std::vector<ElectricPulse> &electric_pulses,
    const double time_slice_duration,
    const double arrival_time_std,
    relleums::random::Generator* prng);

std::vector<std::vector<ExtractedPulse>> extract_pulses(
    const std::vector<std::vector<ElectricPulse>> &electric_pulses,
    const double time_slice_duration,
    const double arrival_time_std,
    relleums::random::Generator* prng);

}  // namespace signal_processing

#endif  // SIGNALPROCESSING_PULSE_EXTRACTION_H_
