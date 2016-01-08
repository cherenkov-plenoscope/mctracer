#include "Tools.h"
//------------------------------------------------------------------------------
namespace Function {

	LinInterpol get_integral(const Func1D &f, uint steps) {

		double step = f.get_limits().get_range()/double(steps);
		std::vector<std::vector<double>> F;

		std::vector<double> F_start = {f.get_limits().get_lower(), 0.0};
		F.push_back(F_start);

		for(uint i=1; i<steps; i++) {
			double x = f.get_limits().get_lower() + i*step;
			double y = f(x)*step + F.back()[1];

			std::vector<double> F_at_x = {x, y};
			F.push_back(F_at_x);
		}

		double x_end = f.get_limits().get_upper();
		std::vector<double> F_end = {x_end, f(x_end-1e-9)*step + F.back()[1]};
		F.push_back(F_end);

		return LinInterpol(F);
	}
	//--------------------------------------------------------------------------
	LinInterpol get_inverse(const Func1D &f, uint steps) {

		double step = f.get_limits().get_range()/double(steps);

		std::vector<std::vector<double>> fvec_inv;

		std::vector<double> f_inv_start = {
			f(f.get_limits().get_lower()), 
			f.get_limits().get_lower()
		};
		fvec_inv.push_back(f_inv_start);

		for(uint i=1; i<steps; i++) {
			double x = f.get_limits().get_lower() + i*step;
			double y = f(x);

			std::vector<double> f_inv_at_x = {y, x};
			fvec_inv.push_back(f_inv_at_x);
		}

		double x_end = f.get_limits().get_upper();
		std::vector<double> f_inv_end = {f(x_end-1e-9), x_end};
		fvec_inv.push_back(f_inv_end);

		return LinInterpol(fvec_inv);
	}
	//--------------------------------------------------------------------------
	LinInterpol get_derivative(const Func1D &f, uint steps) {

		double step = f.get_limits().get_range()/double(steps);
		std::vector<std::vector<double>> fvec_deriv;

		for(uint i=0; i<(steps-1); i++) {
			double x = f.get_limits().get_lower() + i*step;
			double y = (f(x+step) - f(x))/step;

			std::vector<double> f_inv_at_x = {x, y};
			fvec_deriv.push_back(f_inv_at_x);
		}

		double x_end = f.get_limits().get_upper();
		double y_end = (f(x_end-1e-9) - f(x_end-1e-9-step))/step;
		std::vector<double> f_inv_end = {x_end, y_end};
		fvec_deriv.push_back(f_inv_end);

		return LinInterpol(fvec_deriv);
	}
	//--------------------------------------------------------------------------
	bool value_flips_sign(const Func1D &f, uint steps) {

		double step = f.get_limits().get_range()/double(steps);
		bool flip = false;

		for(uint i=0; i<(steps-1); i++) {
			double x = f.get_limits().get_lower() + i*step;
			if( (f(x)>=0.0) != (f(x+step)>=0.0) )
				flip = true;
		}

		return flip;
	}
	//--------------------------------------------------------------------------
} // Function
