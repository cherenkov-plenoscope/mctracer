// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_RANDOM_RANDOM_H_
#define MERLICT_RANDOM_RANDOM_H_

#include "Generator.h"
#include "Mt19937.h"
#include "FakeConstant.h"
#include "SamplesFromDistribution.h"
#include "SpherePointPicker.h"

namespace merlict {
namespace random {

static Mt19937 VOID_PRNG = Mt19937(0u);

}  // namespace random
}  // namespace merlict

#endif  // MERLICT_RANDOM_RANDOM_H_
