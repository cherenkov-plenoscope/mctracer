// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_GENERATOR_H_
#define CORE_RANDOM_GENERATOR_H_

#include <stdint.h>
#include "Core/Vec3.h"

namespace Random {

class Generator {
 protected:
    uint32_t seed_;

 public:
    uint32_t seed()const;
    virtual double uniform() = 0;
    virtual void set_seed(const uint32_t seed);
    Vec3 get_point_on_xy_disc_within_radius(const double r);
    Vec3 get_point_on_xy_disc_within_radius_slow(const double r);
    double expovariate(const double rate);
    virtual double normal(const double mean, const double std_dev) = 0;
};

}  // namespace Random

#endif  // CORE_RANDOM_GENERATOR_H_
