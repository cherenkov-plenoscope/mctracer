// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
#define CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_

#include "random.h"
#include "Core/Function/Function.h"

namespace relleums {
namespace random {

class SamplesFromDistribution {
    const function::Func1* distribution;
    double total_integral_of_distribution;
    function::Func1* cumulative_distribution_function;
 public:
    explicit SamplesFromDistribution(
        const function::Func1* _distribution);
    ~SamplesFromDistribution();
    double draw(const double uniform_0_to_1)const;
    double get_total_integral_of_distribution()const;
};

}  // namespace random
}  // namespace relleums

#endif  // CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
