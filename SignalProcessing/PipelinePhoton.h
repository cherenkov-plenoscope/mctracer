//=================================
// include guard
#ifndef __PipelinePhoton_H_INCLUDED__
#define __PipelinePhoton_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include "PhotonSensor/PhotonSensor.h"
using std::vector;

namespace SignalProcessing {

struct PipelinePhoton {
    double arrival_time;
    double wavelength;
    int simulation_truth_id;
    PipelinePhoton();
    PipelinePhoton(
        const double arrival_time,
        const double wavelength,
        const int simulation_truth_id
    );
};

std::vector<std::vector<PipelinePhoton>> get_photon_pipelines(
    const PhotonSensors::Sensors* sensors
);

void sort_photon_pipelines_arrival_time(std::vector<PipelinePhoton>* pipeline);

}//SignalProcessing
#endif // __PipelinePhoton_H_INCLUDED__