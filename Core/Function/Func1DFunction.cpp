#include "Func1DFunction.h"
#include <sstream>
#include "TracerException.h"

namespace Function {

	Func1D::Func1D(const Limits &_limits): limits(_limits){
		
	}
	//--------------------------------------------------------------------------
 	std::vector<std::vector<double>>Func1D::get_samples(const uint N)const {
		
		double arg = limits.get_lower();
		const double increment = increment_for_steps(N);
		
		std::vector<std::vector<double>> table;
		table.reserve(N);

		for(uint i=0; i<N; i++) {

			const double value = (*this)(arg);
			std::vector<double> row = {arg, value};

			table.push_back(row);
			arg = arg + increment;
		}

		return table;
	}
	//--------------------------------------------------------------------------
	double Func1D::increment_for_steps(const uint N)const {
		return limits.get_range()/double(N);
	}
	//--------------------------------------------------------------------------
	Limits Func1D::get_limits()const {
		return limits;
	}
} // namespace Function