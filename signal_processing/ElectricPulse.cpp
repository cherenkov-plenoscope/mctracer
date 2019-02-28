// Copyright 2014 Sebastian A. Mueller
#include "signal_processing/ElectricPulse.h"
#include "signal_processing/simulation_truth.h"

namespace signal_processing {

ElectricPulse::ElectricPulse():
    simulation_truth_id(DEFAULT_SIMULATION_TRUTH)
{}

ElectricPulse::ElectricPulse(
    const double _arrival_time,
    const int32_t _simulation_truth_id
):
    arrival_time(_arrival_time),
    simulation_truth_id(_simulation_truth_id)
{}

}  // namespace signal_processing
