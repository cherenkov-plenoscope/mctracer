//==============================================================================
// include guard
#ifndef __FUNCTIONFUNC1D_H_INCLUDED__
#define __FUNCTIONFUNC1D_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "LimitsFunction.h"
#include <iostream>
#include <vector>
#include <string>

namespace Function {

	class Func1D {
	protected:

		Limits limits;
	public:

		Func1D() {};
		Func1D(const Limits &_limits);
		void set_limits(const Limits limits);
		virtual ~Func1D();
		std::vector<std::vector<double>> get_samples(const uint N)const;
		double get_mean(const uint N)const;
		virtual double operator()(const double x)const =0;	
		Limits get_limits()const;
		virtual std::string str()const;
	protected:

		double increment_for_steps(const uint N)const;
	};
} // namespace Function
#endif // __FUNCTIONFUNC1D_H_INCLUDED__
