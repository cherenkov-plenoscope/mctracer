#include "ElectricPulse.h"
#include "Core/SimulationTruth.h"
//------------------------------------------------------------------------------
ElectricPulse::ElectricPulse(): 
    simulation_truth_id(SimulationTruth::MCTRACER) 
{}
//------------------------------------------------------------------------------
ElectricPulse::ElectricPulse(
    const double _arrival_time,
    const int _simulation_truth_id
): 
    arrival_time(_arrival_time),
    simulation_truth_id(_simulation_truth_id)
{}
//------------------------------------------------------------------------------