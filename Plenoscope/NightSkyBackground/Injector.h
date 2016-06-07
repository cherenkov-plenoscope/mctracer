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

namespace Plenoscope {
	namespace NightSkyBackground {

	void inject_nsb_into_photon_pipeline(
		std::vector<std::vector<PipelinePhoton>> *photon_pipelines,
		const double exposure_time,
		const std::vector<std::vector<double>> *lft_calib_result,
		const Light *nsb,
		Random::Generator* prng
	);
}// NightSkyBackground
}// Plenoscope
#endif // __PlenoscopeNightSkyBackgroundInjector_H_INCLUDED__ 
