//==============================================================================
// include guard
#ifndef __FUNCTIONLIMITS_H_INCLUDED__
#define __FUNCTIONLIMITS_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies

namespace Function {

	class Limits{
	
		double lower;
		double upper;
	public:
		static const Limits void_limits;
		Limits();
		Limits(const double _lower, const double _upper);
		void assert_limits_are_causal()const;
		double get_lower()const;
		double get_upper()const;
		double get_range()const;
		void assert_contains(const double x)const;
	};
} // namespace Function
#endif // __FUNCTIONLIMITS_H_INCLUDED__
