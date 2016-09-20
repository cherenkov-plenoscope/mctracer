//=================================
// include guard
#ifndef __LensMakerApproximation_H_INCLUDED__
#define __LensMakerApproximation_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/TracerException.h"
#include "LensMaker.h"
//=================================

namespace LensMaker {
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
	}// Approximation

	class NoConvergence : public TracerException {
		using TracerException::TracerException;
	};
}
#endif // __LensMakerApproximation_H_INCLUDED__ 