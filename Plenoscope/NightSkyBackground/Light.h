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

	class Light: public Printable {
	
		const Function::Func1D* nsb_flux_vs_wavelength;
		Random::SamplesFromDistribution nsb_cdf;
		double overall_nsb_rate;
		double solid_angle;
		double area;
		double max_principal_aperture_radius_to_trow_photons_in;
		double max_tilt_vs_optical_axis_to_throw_photons_in;
	public:
		const LightFieldSensor::Geometry *sensor_geometry;
		Light(
			const LightFieldSensor::Geometry *_sensor_geometry, 
			const Function::Func1D* nsb_flux_vs_wavelength
		);
		vector<Photon> get_photons_in_duration(
			const double delay,
			const double duration,
			Random::Generator* prng
		);
		double draw_wavelength(Random::Generator* prng)const;
		string get_print()const;
		double get_overall_rate()const;
	private:
		vector<double> draw_relative_arrival_times(
			const double duration,
			Random::Generator* prng
		);
		Photon get_photon_on_principal_aperture(
			double time_until_reaching_principal_aperture,
			Random::Generator* prng
		)const;
	};
	
	}// NightSkyBackground
}// Plenoscope
#endif // __PlenoscopeNightSkyBackgroundPhotons_H_INCLUDED__ 
