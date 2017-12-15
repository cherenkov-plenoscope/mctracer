// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_RANDOM_RANDOM_H_
#define CORE_RANDOM_RANDOM_H_

#include "Generator.h"
#include "Mt19937.h"
#include "FakeConstant.h"
#include "SamplesFromDistribution.h"

namespace Random {

static const unsigned int ZERO_SEED = 0;
static Mt19937 void_generator = Mt19937(ZERO_SEED);

}  // namespace Random

#endif  // CORE_RANDOM_RANDOM_H_
