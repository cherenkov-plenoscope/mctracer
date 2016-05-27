#include "Plenoscope/Config.h"
#include <functional>

namespace Plenoscope {

	Config::Config() {

		max_FoV_diameter = Deg2Rad(6.5);
		pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
		housing_overhead = 1.2;
		lens_refraction = &pmma_refraction;
		lens_absorbtion = &perfect_transparency;
		sub_pixel_on_pixel_diagonal = 10;
		object_distance_to_focus_on = 10.0e3;
	}
}