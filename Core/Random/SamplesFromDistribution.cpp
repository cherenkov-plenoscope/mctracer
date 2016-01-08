#include "SamplesFromDistribution.h"
//------------------------------------------------------------------------------
namespace Random {
//------------------------------------------------------------------------------
SamplesFromDistribution::SamplesFromDistribution(
	const Function::Func1D* _distribution, 
	Generator* _prng
): distribution(_distribution), prng(_prng) {

	Function::LinInterpol integral_of_distribution = 
		Function::get_integral(*distribution);

	total_integral_of_distribution = 
		integral_of_distribution.get_max_value();

	cumulative_distribution_function = new Function::LinInterpol(
		Function::get_inverse(integral_of_distribution)
	);
}
//------------------------------------------------------------------------------
SamplesFromDistribution::~SamplesFromDistribution() {
	delete cumulative_distribution_function;
}
//------------------------------------------------------------------------------
double SamplesFromDistribution::draw() {

	double y = prng->uniform()*total_integral_of_distribution;
	return (*cumulative_distribution_function)(y);
}
//------------------------------------------------------------------------------
} // Random