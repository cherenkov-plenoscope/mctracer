//==============================================================================
// include guard
#ifndef __FUNCTIONTOOLS_H_INCLUDED__
#define __FUNCTIONTOOLS_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Function.h"

namespace Function {

	static const unsigned int default_steps = 4096;
	LinInterpol get_integral(const Func1D &f, unsigned int steps=default_steps);
	LinInterpol get_inverse(const Func1D &f, unsigned int steps=default_steps);
	LinInterpol get_derivative(const Func1D &f, unsigned int steps=default_steps);
	bool value_flips_sign(const Func1D &f, unsigned int steps=default_steps);
} // Function
#endif // __FUNCTIONTOOLS_H_INCLUDED__
