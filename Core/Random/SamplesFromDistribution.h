//==============================================================================
// include guard
#ifndef __SamplesFromDistribution_H_INCLUDED__
#define __SamplesFromDistribution_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Random.h"
#include "Core/Function/Function.h"
//==============================================================================

// the Pseudo random generators
namespace Random {

	class SamplesFromDistribution {

		const Function::Func1D* distribution;

		double total_integral_of_distribution;
		Function::LinInterpol* cumulative_distribution_function;
	public:

		SamplesFromDistribution(
			const Function::Func1D* _distribution
		);
		~SamplesFromDistribution();
		double draw(const double uniform_0_to_1)const;
		double get_total_integral_of_distribution()const;
	};

} // Random
#endif // __SamplesFromDistribution_H_INCLUDED__