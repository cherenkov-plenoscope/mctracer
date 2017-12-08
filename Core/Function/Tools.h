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

	static const unsigned int DEFAULT_STEPS = 4096;
	LinInterpol get_integral(const Func1D &f, unsigned int steps=DEFAULT_STEPS);
	LinInterpol get_inverse(const Func1D &f, unsigned int steps=DEFAULT_STEPS);
	LinInterpol get_derivative(const Func1D &f, unsigned int steps=DEFAULT_STEPS);
	bool value_flips_sign(const Func1D &f, unsigned int steps=DEFAULT_STEPS);
} // Function
#endif // __FUNCTIONTOOLS_H_INCLUDED__
