#include "FunctionLimits.h"
#include <sstream>
#include "TracerException.h"

namespace Function {
	const Limits Limits::void_limits = Limits(0.0, 0.0);
	//--------------------------------------------------------------------------
	Limits::Limits(const double _lower, const double _upper) {
		lower = _lower;
		upper = _upper;
		assert_limits_are_causal();
	}
	//--------------------------------------------------------------------------
	Limits::Limits() {
		lower = 0.0;
		upper = 0.0;
		assert_limits_are_causal();
	}
	//--------------------------------------------------------------------------
	void Limits::assert_limits_are_causal()const {
		if(upper < lower) {
			std::stringstream info;
			info << "Numerical Function Limit in file " << __FILE__ << "\n";
			info << "Expected upper limit u to be ";
			info << "equal or larger the lower limit l, but actual ";
			info << "u=" << upper << " , l=" << lower << "\n";
			throw TracerException(info.str());
		}
	}
	//--------------------------------------------------------------------------
	double Limits::get_lower()const {
		return lower;
	}
	//--------------------------------------------------------------------------
	double Limits::get_upper()const {
		return upper;
	}
	//--------------------------------------------------------------------------
	double Limits::get_range()const {
		return upper - lower;
	}
	//--------------------------------------------------------------------------
	void Limits::assert_contains(const double x)const {
		if(x < lower || x >= upper) {
			std::stringstream info;
			info << "Numerical Function 1D in file " << __FILE__ << "\n";
			info << "Expected function argument x ";
			info << "to be in limits " << lower << "<= x <";
			info << upper << " but actual x=" << x << "\n";
			throw TracerException(info.str());
		}	
	}
	//--------------------------------------------------------------------------
	std::string Limits::get_print()const {
		std::stringstream out;
		out << "[" << lower << ", " << upper << ")";
		return out.str();
	}
}