#include "Approximation.h"

#include <sstream>
#include <math.h>

#include "Core/Photons.h"
#include "Tools/AsciiIo.h"

namespace LensMaker {
	namespace Approximation {
//------------------------------------------------------------------------------
double get_curvature_radius(const Config &cfg) {

	double R = cfg.focal_length; // iteration start point
	double deviation = 0.0;
	uint iteration_conter = 0;

	do {
		abort_if_too_many_iterations(cfg, ++iteration_conter, R);
		
		// adjustment on curvature radius 
		R = R - deviation*0.1;
		
		// expected focal length for adjusted curvature radius
		double f_expected = get_expected_focal_length_for_R_t_n(
			R, 
			get_lens_thickness_for_R_r(R, cfg.aperture_radius),  
			cfg.refractive_index
		);

		deviation = f_expected - cfg.focal_length;
	}while(fabs(deviation) > 1e-6);

	return R;
}
//------------------------------------------------------------------------------
double get_expected_focal_length_for_R_t_n(
	const double R, const double t, const double n
) {
	double f_inv = (n-1.0)*( 2.0/R - ((n-1.0)*t)/(n*R*R));
	return 1.0/f_inv;
}
//------------------------------------------------------------------------------
double get_lens_thickness_for_R_r(const double R, const double r) {
	return 2.0*R - 2.0*sqrt(R*R - r*r);
}
//------------------------------------------------------------------------------
void abort_if_too_many_iterations(
	const Config &cfg, 
	const uint iteration_conter,
	const double R
) {
	const uint max_iterations = 1000;
	if(iteration_conter > max_iterations) {
		std::stringstream info;
		info << "LensMaker::" << __func__ << "()\n";
		info << "Exceeded max number of " << max_iterations;
		info << " iterations. ";
		info << "Can not reach best curvature radius.\n";
		info << "Input:\n";
		info << "  focal length..........." << cfg.focal_length <<"m\n";
		info << "  aperture radius........" << cfg.aperture_radius <<"m\n";
		info << "  refractive index......." << cfg.refractive_index <<"\n";
		info << "Result:\n";
		info << "  lens curvature radius.." << R <<"m\n";
		info << "  lens thickness........." << get_lens_thickness_for_R_r(
			R,cfg.aperture_radius
		) <<"m\n";
		throw NoConvergence(info.str());
	}
}
//------------------------------------------------------------------------------
	} 
}