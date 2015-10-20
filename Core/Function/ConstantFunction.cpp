#include "ConstantFunction.h"
#include <sstream>
#include "TracerException.h"

namespace Function {

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