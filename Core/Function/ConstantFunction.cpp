#include "ConstantFunction.h"
#include <sstream>
#include "Core/TracerException.h"

namespace Function {
	const Constant Constant::void_function = Constant(0.0, Limits::void_limits);
	//--------------------------------------------------------------------------
	Constant::Constant(): function_value(0.0) {

	}
	//--------------------------------------------------------------------------
	Constant::Constant(const double val, const Limits &_limits)
		:function_value(val) 
	{
		limits = _limits; 
	}
	//--------------------------------------------------------------------------
	void Constant::set_value(const double val) {
		function_value = val;
	}
	//--------------------------------------------------------------------------
	double Constant::operator()(const double x)const {
		limits.assert_contains(x);
		return function_value;
	}
	//--------------------------------------------------------------------------
} // namespace Function