//==============================================================================
// include guard
#ifndef __FUNCTIONCONSTANT_H_INCLUDED__
#define __FUNCTIONCONSTANT_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Func1DFunction.h"

namespace Function {

	class Constant :public Func1D {

		double function_value;
	public:
		Constant();
		Constant(const double val, const Limits &_limit);
		void set_value(const double val);
		double operator()(const double x)const;
		static const Constant void_function;
	};

} // namespace Function
#endif // __FUNCTIONCONSTANT_H_INCLUDED__
