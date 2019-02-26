// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_FAKECONSTANT_H_
#define CORE_RANDOM_FAKECONSTANT_H_

#include "Generator.h"

namespace merlict {
namespace random {

class FakeConstant : public Generator{
    // Gives always the same value.
    double constant;

 public:
    explicit FakeConstant(const double _constant);
    double uniform();
    uint64_t create_seed();
    double normal(const double mean, const double std_dev);
};

}  // namespace random
}  // namespace relleums

#endif  // CORE_RANDOM_FAKECONSTANT_H_
