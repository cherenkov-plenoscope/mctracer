//==============================================================================
// include guard
#ifndef __FUNCTIONCONCATENATE_H_INCLUDED__
#define __FUNCTIONCONCATENATE_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Func1DFunction.h"

namespace Function {

	class Concat : public Func1D {

		std::vector<const Func1D*> conc;
	public:

		Concat();
		Concat(const std::vector<const Func1D*> _conc);
		void set_functions(const std::vector<const Func1D*> _conc);
		double operator()(const double x)const;
	private:

		void assert_limits_do_fit()const;
		void adopt_new_limits();
		const Func1D* get_sub_function_responsible_for(const double x)const;
		static bool compare_upper_limit(const double x, const Func1D *f);
		bool func_does_not_match_limit_of_next_func(const uint i)const;
	};
} // namespace Function
#endif // __FUNCTIONCONCATENATE_H_INCLUDED__
