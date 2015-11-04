#include "LensMaker.h"
#include <sstream>
#include <math.h>
//------------------------------------------------------------------------------
//  f: focal length
//  R: curvature radius(absolute the same for both caps of the lens)
//  n: refractive index
//  r: outer aperture radius of the lens
//
//  The lens maker equation
//  1/f = (n-1) [2/R - ((n-1)t)/(nR^2)]
//  
//  thickness of lens
//  t = 2R - 2sqrt(R^2 - r^2)
//------------------------------------------------------------------------------
namespace LensMaker {
	//--------------------------------------------------------------------------
	double get_curvature_radius_given_f_r_n(
		const double f, const double r, const double n
	) {
		double R = f; // iteration start point
		double deviation = 0.0;
		uint iteration_conter = 0;

		do {
			abort_if_too_many_iterations_f_R_r_n(++iteration_conter,f,R,r,n);
			
			// adjustment on curvature radius 
			R = R - deviation*0.1;
			
			// expected focal length for adjusted curvature radius
			double f_expected = get_expected_focal_length_for_R_t_n(
				R, get_lens_thickness_for_R_r(R, r), n);

			deviation = f_expected - f;
		}while(fabs(deviation) > 1e-6);

		return R;
	}
	//--------------------------------------------------------------------------
	double get_expected_focal_length_for_R_t_n(
		const double R, const double t, const double n
	) {
		double f_inv = (n-1.0)*( 2.0/R - ((n-1.0)*t)/(n*R*R));
		return 1.0/f_inv;
	}
	//--------------------------------------------------------------------------
	double get_lens_thickness_for_R_r(const double R, const double r) {
		return 2.0*R - 2.0*sqrt(R*R - r*r);
	}
	//--------------------------------------------------------------------------
	void abort_if_too_many_iterations_f_R_r_n(const uint iteration_conter,
		const double f, const double R, const double r,  const double n
	) {
		const uint max_iterations = 1000;
		if(iteration_conter > max_iterations) {
			std::stringstream info;
			info << "LensMaker::" << __func__ << "()\n";
			info << "Exceeded max number of " << max_iterations;
			info << " iterations. ";
			info << "Can not reach best curvature radius.\n";
			info << "Input:\n";
			info << "  focal length..........." << f <<"m\n";
			info << "  aperture radius........" << r <<"m\n";
			info << "  refractive index......." << n <<"\n";
			info << "Result:\n";
			info << "  lens curvature radius.." << R <<"m\n";
			info << "  lens thickness........." << get_lens_thickness_for_R_r(R,r) <<"m\n";
			throw TracerException(info.str());
		}
	}
	//--------------------------------------------------------------------------
} // LensMaker