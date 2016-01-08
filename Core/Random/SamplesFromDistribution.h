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
		Generator* prng;

		double total_integral_of_distribution;
		Function::LinInterpol* cumulative_distribution_function;
	public:

		SamplesFromDistribution(
			const Function::Func1D* _distribution, 
			Generator* prng
		);
		~SamplesFromDistribution();
		double draw();
	};

} // Random
#endif // __SamplesFromDistribution_H_INCLUDED__