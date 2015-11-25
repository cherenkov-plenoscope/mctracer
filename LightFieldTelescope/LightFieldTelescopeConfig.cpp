#include "LightFieldTelescope.h"

namespace LightFieldTelescope {
	//--------------------------------------------------------------------------
	Config::Config() {

		max_FoV_diameter = Deg2Rad(6.5);
		pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
		housing_overhead = 1.2;
		lens_refraction = &pmma_refraction;
		sub_pixel_per_pixel = 100;
	}
}