//=================================
// include guard
#ifndef __PlenoscopeNightSkyBackgroundInjector_H_INCLUDED__
#define __PlenoscopeNightSkyBackgroundInjector_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/NightSkyBackground/Light.h"
#include "SignalProcessing/PipelinePhoton.h"
#include "Plenoscope/Calibration/LixelStatistics.h"
using std::vector;

namespace Plenoscope {
	namespace NightSkyBackground {

	void inject_nsb_into_photon_pipeline(
		vector<vector<PipelinePhoton>> *photon_pipelines,
		const double exposure_time,
		const vector<Plenoscope::Calibration::LixelStatistic> *lixel_statistics,
		const Light *nsb,
		Random::Generator* prng
	);
	}// NightSkyBackground
}// Plenoscope
#endif // __PlenoscopeNightSkyBackgroundInjector_H_INCLUDED__ 
