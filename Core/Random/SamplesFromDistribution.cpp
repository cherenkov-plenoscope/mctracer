// Copyright 2014 Sebastian A. Mueller
#include "Core/Random/SamplesFromDistribution.h"

namespace relleums {
namespace Random {

SamplesFromDistribution::SamplesFromDistribution(
    const Function::Func1* _distribution
): distribution(_distribution) {
    Function::Func1 integral_of_distribution =
        Function::get_integral(*distribution);

    total_integral_of_distribution =
        integral_of_distribution.max();

    cumulative_distribution_function = new Function::Func1(
        Function::get_inverse(integral_of_distribution));
}

SamplesFromDistribution::~SamplesFromDistribution() {
    delete cumulative_distribution_function;
}

double SamplesFromDistribution::draw(const double uniform_0_to_1)const {
    double y = uniform_0_to_1*total_integral_of_distribution;
    return cumulative_distribution_function->evaluate(y);
}

double SamplesFromDistribution::get_total_integral_of_distribution()const {
    return total_integral_of_distribution;
}

}  // namespace Random
}  // namespace relleums
