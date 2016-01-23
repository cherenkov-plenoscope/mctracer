//=================================
// include guard
#ifndef __LightFieldTelescopeConfig_H_INCLUDED__
#define __LightFieldTelescopeConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Geometry/SegmentedReflector/SegmentedReflector.h"

namespace LightFieldTelescope {

	struct Config {

		SegmentedReflector::Config reflector;

		double max_FoV_diameter;
		double pixel_FoV_hex_flat2flat;	
		double housing_overhead;
		uint sub_pixel_on_pixel_diagonal;

		double object_distance_to_focus_on;

		const Function::Func1D* lens_refraction;
		const Function::Func1D* lens_absorbtion; 
		Config();
	};

	static const Function::Constant pmma_refraction(
		1.49, 
		Function::Limits(200e-9, 1200e-9)
	);
}
#endif // __LightFieldTelescopeConfig_H_INCLUDED__ 
