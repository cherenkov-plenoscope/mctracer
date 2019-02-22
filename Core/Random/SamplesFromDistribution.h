// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
#define CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_

#include "Random.h"
#include "Core/Function/Function.h"

namespace relleums {
namespace Random {

class SamplesFromDistribution {
    const Function::Func1D* distribution;
    double total_integral_of_distribution;
    Function::Func1D* cumulative_distribution_function;
 public:
    explicit SamplesFromDistribution(
        const Function::Func1D* _distribution);
    ~SamplesFromDistribution();
    double draw(const double uniform_0_to_1)const;
    double get_total_integral_of_distribution()const;
};

}  // namespace Random
}  // namespace relleums

#endif  // CORE_RANDOM_SAMPLESFROMDISTRIBUTION_H_
