#include "PipelinePhoton.h"
#include <algorithm>

PipelinePhoton::PipelinePhoton() {
	arrival_time = 0.0;
	wavelength = 0.0;
}

std::vector<std::vector<PipelinePhoton>> get_photon_pipelines(
	const PhotonSensors::Sensors* sensors
) {
	std::vector<std::vector<PipelinePhoton>> photon_pipelines;
	const uint number_sensors = sensors->by_occurence.size();
	photon_pipelines.reserve(number_sensors);

	// for each sensor
	for(uint i=0; i<number_sensors; i++) {
		
		std::vector<PipelinePhoton> photon_pipeline;
		const uint number_photons = 
			sensors->by_occurence[i]->arrival_table.size();
		photon_pipeline.reserve(number_photons);

		// for each photon 
		for(uint j=0; j<number_photons; j++) {
			
			PipelinePhoton ph;
			ph.arrival_time = 
				sensors->by_occurence[i]->arrival_table[j].arrival_time;
			ph.wavelength =
				sensors->by_occurence[i]->arrival_table[j].wavelength;

			photon_pipeline.push_back(ph);
		}

		sort_photon_pipelines_arrival_time(&photon_pipeline);

		photon_pipelines.push_back(photon_pipeline);
	}

	return photon_pipelines;
}

void sort_photon_pipelines_arrival_time(std::vector<PipelinePhoton>* pipeline) {
    
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