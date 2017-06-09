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
		double fov_radius;
		double fov_solid_angle;
		double aperture_radius;
		double aperture_area;
		const LightFieldSensor::Geometry *sensor_geometry;
		Light(
			const LightFieldSensor::Geometry *_sensor_geometry, 
			const Function::Func1D* flux_vs_wavelength
		);
		string __repr__()const;
	};
	
	}// NightSkyBackground
}// Plenoscope
#endif // __PlenoscopeNightSkyBackgroundPhotons_H_INCLUDED__ 
