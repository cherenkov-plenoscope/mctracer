// Copyright 2014 Sebastian A. Mueller
#include "signal_processing/ElectricPulse.h"
#include "merlict/merlict.h"

namespace signal_processing {

ElectricPulse::ElectricPulse():
    simulation_truth_id(merlict::SimulationTruth::MCTRACER_DEFAULT)
{}

ElectricPulse::ElectricPulse(
    const double _arrival_time,
    const int32_t _simulation_truth_id
):
    arrival_time(_arrival_time),
    simulation_truth_id(_simulation_truth_id)
{}

}  // namespace signal_processing
