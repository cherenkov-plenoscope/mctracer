// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_PIPELINEPHOTON_H_
#define MCTRACER_SIGNALPROCESSING_PIPELINEPHOTON_H_

#include <stdint.h>
#include <vector>
#include "PhotonSensor/PhotonSensor.h"

namespace SignalProcessing {

struct PipelinePhoton {
    double arrival_time;
    double wavelength;
    int32_t simulation_truth_id;
    PipelinePhoton();
    PipelinePhoton(
        const double arrival_time,
        const double wavelength,
        const int32_t simulation_truth_id);
};

std::vector<std::vector<PipelinePhoton>> get_photon_pipelines(
    const PhotonSensor::Sensors* sensors);

void sort_photon_pipelines_arrival_time(
    std::vector<PipelinePhoton>* pipeline);

}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_PIPELINEPHOTON_H_
