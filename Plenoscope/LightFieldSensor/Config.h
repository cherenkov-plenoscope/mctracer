//=================================
// include guard
#ifndef __PlenoscopeLightFieldSensorConfig_H_INCLUDED__
#define __PlenoscopeLightFieldSensorConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Function/Function.h"
#include <limits>
#include "Tools/Tools.h"

namespace Plenoscope {

	namespace LightFieldSensor {

		struct Config {
			// The imaging system's expected properties as focal length and max
			// outer aperture radius do not neccessarily match the dimensions 
			// of the actual imaging system which will be used.
			// For the best performance of the plenoscope, the expected 
			// dimensions should match the actual ones of the imaging system, 
			// but to study imperfect imaging systems, where the dimensions
			// differ on purpose, the expected dimensions can differ from 
			// the actual ones.
			double expected_imaging_system_focal_length;
			double expected_imaging_system_max_aperture_radius;
			double max_FoV_diameter;
			double pixel_FoV_hex_flat2flat;	
			double housing_overhead;
			unsigned int number_of_paxel_on_pixel_diagonal;
			const Function::Func1D* lens_refraction;
			const Function::Func1D* lens_absorbtion;
			const Function::Func1D* bin_relection;
			Config(); 
		};
	
		static const Function::Constant pmma_refraction(
			1.49, Function::Limits(200e-9, 1200e-9)
		);

		static const Function::Constant perfect_transparency(
			std::numeric_limits<double>::infinity(), Function::Limits(200e-9, 1200e-9)
		);
	}
}
#endif // __PlenoscopeLightFieldSensorConfig_H_INCLUDED__ 
