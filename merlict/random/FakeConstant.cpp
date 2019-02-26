// Copyright 2014 Sebastian A. Mueller
#include "merlict/random/FakeConstant.h"
#include "random.h"

namespace merlict {
namespace random {

FakeConstant::FakeConstant(const double _constant) {
    constant = _constant;
    seed_ = ZERO_SEED;
}

double FakeConstant::uniform() {
    return constant;
}

double FakeConstant::normal(const double mean, const double std_dev) {
    (void)mean;
    (void)std_dev;
    return constant;
}

uint64_t FakeConstant::create_seed() {
	return 0u;
}

}  // namespace random
}  // namespace relleums
