//=================================
// include guard
#ifndef __LensMaker_H_INCLUDED__
#define __LensMaker_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "TracerException.h"
#include "Core/TracerSettings.h"

//=================================

namespace LensMaker {

	struct Config {
		double focal_length;
		double aperture_radius;
		double refractive_index;

		Config();
	};

	class Itterative {

		const Config order;
		double curvature_radius_1st_guess;
		double curvature_radius;
		uint max_number_of_iterations;
		uint number_of_photons_per_iteration;
		TracerSettings settings;
		double last_point_spread_std_dev;
		double last_curvature_radius;
	public:

		Itterative(const Config wishlist);
		double get_curvature_radius_for_bi_konvex_lens()const;
	private:

		void abort_if_too_many_iterations(const uint i)const;
		bool is_bigger_then_last_psf(const double psf_std_dev);
	};

	namespace Approximation {

		double get_curvature_radius(const Config &cfg);
		
		double get_lens_thickness_for_R_r(const double R, const double r);
		
		void abort_if_too_many_iterations(
			const Config &cfg, 
			const uint iteration_conter,
			const double R
		);
		double get_expected_focal_length_for_R_t_n(
			const double R, const double t, const double n
		);
	}// Analytic

	class NoConvergence : public TracerException {
		using TracerException::TracerException;
	};
}
#endif // __LensMaker_H_INCLUDED__ 