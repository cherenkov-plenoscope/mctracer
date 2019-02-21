// Copyright 2014 Sebastian A. Mueller
#include "SignalProcessing/ElectricPulse.h"
#include "Core/SimulationTruth.h"

namespace SignalProcessing {

ElectricPulse::ElectricPulse():
    simulation_truth_id(relleums::SimulationTruth::MCTRACER_DEFAULT)
{}

ElectricPulse::ElectricPulse(
    const double _arrival_time,
    const int32_t _simulation_truth_id
):
    arrival_time(_arrival_time),
    simulation_truth_id(_simulation_truth_id)
{}

}  // namespace SignalProcessing
