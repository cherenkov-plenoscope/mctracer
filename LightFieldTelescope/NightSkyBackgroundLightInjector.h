//=================================
// include guard
#ifndef __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__
#define __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PipelinePhoton.h"

namespace LightFieldTelescope {

	class NightSkyBackgroundLightInjector {

	public:

		NightSkyBackgroundLightInjector(
			const std::vector<std::vector<double>> *light_field_calibration_result,
			const std::vector<std::vector<PipelinePhoton>> *cherenkov_photons,
		);

	private:

	};
}
#endif // __LightFieldTelescopeNightSkyBackgroundLightInjector_H_INCLUDED__ 
