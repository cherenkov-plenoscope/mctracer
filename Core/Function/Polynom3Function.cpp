#include "Polynom3Function.h"
namespace Function {
	//--------------------------------------------------------------------------
	Polynom3::Polynom3(): a3(0.0), a2(0.0), a1(0.0), a0(0.0) {
	}
	//--------------------------------------------------------------------------
	Polynom3::Polynom3(
		const double a3, 
		const double a2, 
		const double a1, 
		const double a0,
		const Limits &limits 
	) {
		init(a3, a2, a1, a0, limits);
	}
	//--------------------------------------------------------------------------
	void Polynom3::init(
		const double a3, 
		const double a2, 
		const double a1, 
		const double a0,
		const Limits &limits
	) {
		this->a3 = a3;
		this->a2 = a2;
		this->a1 = a1;
		this->a0 = a0;
		this->limits = limits;
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