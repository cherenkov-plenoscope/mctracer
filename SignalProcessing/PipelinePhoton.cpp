#include "PipelinePhoton.h"
#include "Core/SimulationTruth.h"
#include <algorithm>

namespace SignalProcessing {
//------------------------------------------------------------------------------
PipelinePhoton::PipelinePhoton(): 
    simulation_truth_id(SimulationTruth::MCTRACER_DEFAULT) 
{
    arrival_time = 0.0;
    wavelength = 0.0;
}
//------------------------------------------------------------------------------
PipelinePhoton::PipelinePhoton(
    const double _arrival_time,
    const double _wavelength,
    const int _simulation_truth_id
): 
    arrival_time(_arrival_time),
    wavelength(_wavelength),
    simulation_truth_id(_simulation_truth_id)
{}
//------------------------------------------------------------------------------
vector<vector<PipelinePhoton>> get_photon_pipelines(
    const PhotonSensor::Sensors* sensors
) {
    vector<vector<PipelinePhoton>> photon_pipelines;
    const uint number_sensors = sensors->size();
    photon_pipelines.reserve(number_sensors);

    // for each sensor
    for(uint i=0; i<number_sensors; i++) {
        
        vector<PipelinePhoton> photon_pipeline;
        const uint number_photons = 
            sensors->by_occurence[i]->arrival_table.size();
        photon_pipeline.reserve(number_photons);

        // for each photon 
        for(uint j=0; j<number_photons; j++) {
            
            PipelinePhoton ph(
                sensors->by_occurence[i]->arrival_table[j].arrival_time,
                sensors->by_occurence[i]->arrival_table[j].wavelength,
                sensors->by_occurence[i]->arrival_table[j].simulation_truth_id
            );

            photon_pipeline.push_back(ph);
        }

        sort_photon_pipelines_arrival_time(&photon_pipeline);
        photon_pipelines.push_back(photon_pipeline);
    }

    return photon_pipelines;
}
//------------------------------------------------------------------------------
void sort_photon_pipelines_arrival_time(vector<PipelinePhoton>* pipeline) {
    
    std::sort(
        pipeline->begin(), 
        pipeline->end(),
        [&](const PipelinePhoton& a, const PipelinePhoton& b) {
            return (
                a.arrival_time < b.arrival_time
            );
        }
    );
}
//------------------------------------------------------------------------------
}//SignalProcessing