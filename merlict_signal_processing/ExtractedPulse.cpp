// Copyright 2017 Sebastian A. Mueller
#include "merlict_signal_processing/ExtractedPulse.h"
#include "merlict_signal_processing/simulation_truth.h"

namespace signal_processing {

ExtractedPulse::ExtractedPulse():
    simulation_truth_id(DEFAULT_SIMULATION_TRUTH)
{}

ExtractedPulse::ExtractedPulse(
    const uint8_t arrival_time_slice,
    const int32_t _simulation_truth_id
):
    arrival_time_slice(arrival_time_slice),
    simulation_truth_id(_simulation_truth_id)
{}

}  // namespace signal_processing
