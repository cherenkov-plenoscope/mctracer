//=================================
// include guard
#ifndef __PlenoscopeNightSkyBackgroundPhotons_H_INCLUDED__
#define __PlenoscopeNightSkyBackgroundPhotons_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/LightFieldSensor/Geometry.h"
#include "Core/Random/Random.h"
#include "Core/Function/Function.h"
#include "Core/Photon.h"

namespace Plenoscope {
	namespace NightSkyBackground {

	struct Light {
	
		const Function::Func1D* flux_vs_wavelength;
		Random::SamplesFromDistribution wavelength_probability;
		double rate;
		double fov_solid_angle;
		double aperture_area;
		double max_principal_aperture_radius_to_trow_photons_in;
		double max_tilt_vs_optical_axis_to_throw_photons_in;

		const LightFieldSensor::Geometry *sensor_geometry;
		Light(
			const LightFieldSensor::Geometry *_sensor_geometry, 
			const Function::Func1D* flux_vs_wavelength
		);
		double draw_wavelength(Random::Generator* prng)const;
		double get_overall_rate()const;
		string __repr__()const;
	};
	
	}// NightSkyBackground
}// Plenoscope
#endif // __PlenoscopeNightSkyBackgroundPhotons_H_INCLUDED__ 
