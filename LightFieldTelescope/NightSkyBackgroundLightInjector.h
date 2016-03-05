//=================================
// include guard
#ifndef __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__
#define __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "SignalProcessing/PipelinePhoton.h"

namespace LightFieldTelescope {

	void inject_nsb_into_photon_pipeline(
		std::vector<std::vector<PipelinePhoton>> *photon_pipelines,
		const double exposure_time,
		const std::vector<std::vector<double>> *lft_calib_result,
		const NightSkyBackgroundLight *telescope_nsb,
		Random::Generator* prng
	);
}
#endif // __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__ 
