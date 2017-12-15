// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_FAKECONSTANT_H_
#define CORE_RANDOM_FAKECONSTANT_H_

#include "Generator.h"

namespace Random {

class FakeConstant : public Generator{
    // Gives always the same value.
    double constant;

 public:
    explicit FakeConstant(const double _constant);
    double uniform();
    double normal(const double mean, const double std_dev);
};

}  // namespace Random

#endif  // CORE_RANDOM_FAKECONSTANT_H_
