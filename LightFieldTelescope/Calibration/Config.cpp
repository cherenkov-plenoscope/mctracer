#include "Config.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
CalibrationConfig::CalibrationConfig() {
	photons_per_sub_pixel = 25;
	photons_per_block = 1e6;
}
//------------------------------------------------------------------------------
}