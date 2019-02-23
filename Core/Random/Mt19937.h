// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_MT19937_H_
#define CORE_RANDOM_MT19937_H_

#include "Generator.h"
#include <random>

namespace relleums {
namespace Random {

class Mt19937 : public Generator{
    // mersenne_twister_engine
    std::mt19937_64 pRNG_mt19937;
    double inv_max;

 public:
    explicit Mt19937(const uint64_t seed);
    Mt19937();
    double uniform();
    uint64_t create_seed();
    double normal(const double mean, const double std_dev);
    void set_seed(const uint64_t seed);

 private:
    void init_inverse_maximum();
};

}  // namespace Random
}  // namespace relleums

#endif  // CORE_RANDOM_MT19937_H_
