#include "Polynom3Function.h"
namespace Function {
	//--------------------------------------------------------------------------
	Polynom3::Polynom3(
		const double _a3, 
		const double _a2, 
		const double _a1, 
		const double _a0,
		const Limits &_limits 
	): a3(_a3), a2(_a2), a1(_a1), a0(_a0) {
		limits = _limits;
	}
	//--------------------------------------------------------------------------
	double Polynom3::operator()(const double x)const {
		limits.assert_contains(x);
		const double x2 = x*x;
		const double x3 = x2*x;
		return a3*x3 + a2*x2 + a1*x + a0;
	}
	//--------------------------------------------------------------------------
} // namespace Function