//=================================
// include guard
#ifndef __LensMaker_H_INCLUDED__
#define __LensMaker_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "TracerException.h"
//=================================

namespace LensMaker {
	double get_curvature_radius_given_f_r_n(
		const double f, const double r, const double n
	);
	
	double get_lens_thickness_for_R_r(const double R, const double r);
	
	void abort_if_too_many_iterations_f_R_r_n(
		const uint iteration_conter,
		const double f, 
		const double R, 
		const double r, 
		const double n
	);
	double get_expected_focal_length_for_R_t_n(
		const double R, const double t, const double n
	);

	class NoConvergence : public TracerException {
		using TracerException::TracerException;
	};
}
#endif // __LensMaker_H_INCLUDED__ 