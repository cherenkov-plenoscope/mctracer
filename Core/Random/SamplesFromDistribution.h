// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
#define CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_

#include "Random.h"
#include "Core/Function/Function.h"

namespace relleums {
namespace random {

class SamplesFromDistribution {
    const Function::Func1* distribution;
    double total_integral_of_distribution;
    Function::Func1* cumulative_distribution_function;
 public:
    explicit SamplesFromDistribution(
        const Function::Func1* _distribution);
    ~SamplesFromDistribution();
    double draw(const double uniform_0_to_1)const;
    double get_total_integral_of_distribution()const;
};

}  // namespace random
}  // namespace relleums

#endif  // CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
