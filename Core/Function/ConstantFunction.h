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

		double value;
	public:
		Constant();
		Constant(const double value, const Limits &limits);
		void init(const double value, const Limits &limits);
		double evaluate(const double x)const;
		static const Constant VOID_FUNCTION;
	};

} // namespace Function
#endif // __FUNCTIONCONSTANT_H_INCLUDED__
