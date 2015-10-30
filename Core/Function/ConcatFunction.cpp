#include "ConcatFunction.h"
#include <sstream>
#include "TracerException.h"
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort

namespace Function {

	Concat::Concat(const std::vector<const Func1D*> _conc) {
		conc = _conc;
		assert_limits_do_fit();
		adopt_new_limits();
	}
	//--------------------------------------------------------------------------
	void Concat::assert_limits_do_fit()const {

		for(uint i=0; i<conc.size()-1; i++) {
			if(func_does_not_match_limit_of_next_func(i)) {
				std::stringstream info;
				info << "Function::Concat::" << __func__ << "()\n";
				info << __FILE__ << ", " << __LINE__ << "\n";
				info << "Expected limits of functions to be concatenated to fit. ";
				info << "function[" << i << "].upper_limit == ";
				info << "function[" << i+1 << "].lower_limit, but actual\n";
				info << "f[" << i << "].upper_limit = ";
				info << conc.at(i)->get_limits().get_upper() << "\n";
				info << "f[" << i+1 << "].lower_limit = ";
				info << conc.at(i+1)->get_limits().get_lower() << "\n";
				throw Limits::NotCausal(info.str());			
			}
		}
	}
	//--------------------------------------------------------------------------
	bool Concat::func_does_not_match_limit_of_next_func(const uint i)const {
		return conc.at(i)->get_limits().get_upper() != 
			conc.at(i+1)->get_limits().get_lower();
	}
	//--------------------------------------------------------------------------
	void Concat::adopt_new_limits() {
		limits = Limits(
			conc.front()->get_limits().get_lower(),
			conc.back()->get_limits().get_upper()
		);
	}
	//--------------------------------------------------------------------------
	double Concat::operator()(const double x)const {
		limits.assert_contains(x);
		const Func1D* sub_f = get_sub_function_responsible_for(x);
		return (*sub_f)(x);
	}
	//--------------------------------------------------------------------------
	const Func1D* Concat::get_sub_function_responsible_for(const double x)const {
		return	(*	std::upper_bound(
						conc.begin(),
						conc.end(), 
						x, 
						compare_upper_limit
					)
				);	
	}
	//--------------------------------------------------------------------------
	bool Concat::compare_upper_limit(const double x, const Func1D *f) {
		return f->get_limits().get_upper() > x;
	}
} // namespace Function