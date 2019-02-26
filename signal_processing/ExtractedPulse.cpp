// Copyright 2017 Sebastian A. Mueller
#include "signal_processing/ExtractedPulse.h"
#include "merlict/merlict.h"

namespace signal_processing {

ExtractedPulse::ExtractedPulse():
    simulation_truth_id(merlict::SimulationTruth::DEFAULT)
{}

ExtractedPulse::ExtractedPulse(
    const uint8_t arrival_time_slice,
    const int32_t _simulation_truth_id
):
    arrival_time_slice(arrival_time_slice),
    simulation_truth_id(_simulation_truth_id)
{}

}  // namespace signal_processing
