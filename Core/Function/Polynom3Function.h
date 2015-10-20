//==============================================================================
// include guard
#ifndef __FUNCTIONPOLYNOM3_H_INCLUDED__
#define __FUNCTIONPOLYNOM3_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Func1DFunction.h"

namespace Function {
	class Polynom3 : public Func1D {

		const double a3, a2, a1, a0;
		double average;
	public:
		Polynom3(
			const double _a3, 
			const double _a2, 
			const double _a1, 
			const double _a0,
			const Limits &_limits
		);
		double operator()(const double x)const;
	};
} // namespace Function
#endif // __FUNCTIONPOLYNOM3_H_INCLUDED__