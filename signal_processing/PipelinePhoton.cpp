// Copyright 2014 Sebastian A. Mueller
#include "signal_processing/PipelinePhoton.h"
#include <algorithm>
#include "merlict/merlict.h"
using std::vector;

namespace signal_processing {

PipelinePhoton::PipelinePhoton():
    simulation_truth_id(merlict::SimulationTruth::MCTRACER_DEFAULT) {
    arrival_time = 0.0;
    wavelength = 0.0;
}

PipelinePhoton::PipelinePhoton(
    const double _arrival_time,
    const double _wavelength,
    const int32_t _simulation_truth_id
):
    arrival_time(_arrival_time),
    wavelength(_wavelength),
    simulation_truth_id(_simulation_truth_id)
{}

vector<vector<PipelinePhoton>> get_photon_pipelines(
    const merlict::sensor::Sensors* sensors
) {
    vector<vector<PipelinePhoton>> photon_pipelines;
    const unsigned int number_sensors = sensors->size();
    photon_pipelines.reserve(number_sensors);

    // for each sensor
    for (unsigned int i = 0; i < number_sensors; i++) {
        vector<PipelinePhoton> photon_pipeline;
        const unsigned int number_photons =
            sensors->by_occurence[i]->photon_arrival_history.size();
        photon_pipeline.reserve(number_photons);

        // for each photon
        for (unsigned int j = 0; j < number_photons; j++) {
            PipelinePhoton ph(
                sensors->by_occurence[i]->photon_arrival_history[j].
                    arrival_time,
                sensors->by_occurence[i]->photon_arrival_history[j].
                    wavelength,
                sensors->by_occurence[i]->photon_arrival_history[j].
                    simulation_truth_id);

            photon_pipeline.push_back(ph);
        }

        sort_photon_pipelines_arrival_time(&photon_pipeline);
        photon_pipelines.push_back(photon_pipeline);
    }

    return photon_pipelines;
}

void sort_photon_pipelines_arrival_time(vector<PipelinePhoton>* pipeline) {
    std::sort(
        pipeline->begin(),
        pipeline->end(),
        [&](const PipelinePhoton& a, const PipelinePhoton& b) {
            return (a.arrival_time < b.arrival_time);
        });
}

}  // namespace signal_processing
