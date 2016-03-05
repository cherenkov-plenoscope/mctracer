//=================================
// include guard
#ifndef __SegmentedReflectorConfig_H_INCLUDED__
#define __SegmentedReflectorConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Color.h"
#include "Core/Function/Function.h"

namespace SegmentedReflector {

	struct Config {

		double focal_length;
		double DaviesCotton_over_parabolic_mixing_factor;
		double max_outer_aperture_radius;
		double min_inner_aperture_radius;
		double facet_inner_hex_radius;
		double gap_between_facets;	

		const Color *mirror_color;
		const Color *inner_mirror_color;
		const Function::Func1D* reflectivity;

		Config();
	};

	static const Function::Constant typical_reflectivity(
		0.8, 
		Function::Limits(200e-9, 1200e-9)
	);

	static const Function::Constant perfect_reflectivity(
		1.0, 
		Function::Limits(200e-9, 1200e-9)
	);
} // SegmentedReflector
#endif // __SegmentedReflectorConfig_H_INCLUDED__ 