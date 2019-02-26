// Copyright 2014 Sebastian A. Mueller
#include "merlict/random/SamplesFromDistribution.h"

namespace merlict {
namespace random {

SamplesFromDistribution::SamplesFromDistribution(
    const function::Func1* _distribution
): distribution(_distribution) {
    function::Func1 integral_of_distribution =
        function::integral(*distribution);

    total_integral_of_distribution =
        integral_of_distribution.max();

    cumulative_distribution_function = new function::Func1(
        function::inverse(integral_of_distribution));
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

}  // namespace random
}  // namespace relleums
