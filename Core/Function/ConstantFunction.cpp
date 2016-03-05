#include "ConstantFunction.h"
#include <sstream>
#include "Core/TracerException.h"

namespace Function {
	const Constant Constant::void_function = Constant(0.0, Limits::void_limits);
	//--------------------------------------------------------------------------
	Constant::Constant(const double val, const Limits &_limits)
		:function_value(val) 
	{
		limits = _limits; 
	}
	//--------------------------------------------------------------------------
	double Constant::operator()(const double x)const {
		limits.assert_contains(x);
		return function_value;
	}
	//--------------------------------------------------------------------------
} // namespace Function