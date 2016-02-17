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

struct PipelinePhoton {
	double arrival_time;
	double wavelength;
	PipelinePhoton();
};

std::vector<std::vector<PipelinePhoton>> get_photon_pipelines(
	const PhotonSensors::Sensors* sensors
);

void sort_photon_pipelines_arrival_time(std::vector<PipelinePhoton>* pipeline);

//double get_arrival_time_mean()

#endif // __PipelinePhoton_H_INCLUDED__