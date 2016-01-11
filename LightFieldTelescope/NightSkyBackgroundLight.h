//=================================
// include guard
#ifndef __LightFieldTelescopeNightSkyBackgroundLight_H_INCLUDED__
#define __LightFieldTelescopeNightSkyBackgroundLight_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "Core/Random/Random.h"
#include "Core/Function/Function.h"

namespace LightFieldTelescope {

	class NightSkyBackgroundLight: public Printable {

		const Geometry *telescope_geometry;
		const Function::Func1D* nsb_flux_vs_wavelength;
		Random::SamplesFromDistribution nsb_cdf;
		double overall_nsb_rate;
		double solid_angle;
		double area;
		double max_principal_aperture_radius_to_trow_photons_in;
		double max_tilt_vs_optical_axis_to_throw_photons_in;
		std::vector<double> relative_arrival_times;
	public:

		NightSkyBackgroundLight(
			const Geometry *_telescope_geometry, 
			const Function::Func1D* nsb_flux_vs_wavelength
		);
		std::vector<Photon*>* get_photons_in_duration(
			const double delay,
			const double duration,
			Random::Generator* prng
		);
		std::string get_print()const;
	private:
		void init_relative_arrival_times(
			const double duration,
			Random::Generator* prng
		);
		Photon* get_photon_given_pos_incident_dir_wvl_and_time_on_principal_aperture(
			Vector3D pos_on_principal_aperture,
			Vector3D direction_on_principal_aperture,
			double wavelength,
			double moment
		)const;
	};
}
#endif // __LightFieldTelescopeNightSkyBackgroundLight_H_INCLUDED__ 
