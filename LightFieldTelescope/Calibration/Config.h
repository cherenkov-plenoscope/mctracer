//=================================
// include guard
#ifndef __LightFieldTelescopeCalibrationConfig_H_INCLUDED__
#define __LightFieldTelescopeCalibrationConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <stdint.h>

namespace LightFieldTelescope {

	struct CalibrationConfig {
		uint32_t photons_per_sub_pixel;
		uint32_t photons_per_block;

		CalibrationConfig();
	};
} //LightFieldTelescope
#endif // __LightFieldTelescopeCalibrationConfig_H_INCLUDED__ 