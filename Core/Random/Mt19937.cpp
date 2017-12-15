// Copyright 2014 Sebastian A. Mueller
#include "Core/Random/Mt19937.h"
#include "Random.h"

namespace Random {

Mt19937::Mt19937(const uint32_t seed) {
    init_inverse_maximum();
    set_seed(seed);
}

Mt19937::Mt19937() {
    init_inverse_maximum();
    set_seed(Random::ZERO_SEED);
}

double Mt19937::uniform() {
    return static_cast<double>(pRNG_mt19937())*inv_max;
}

void Mt19937::set_seed(const uint32_t seed) {
    this->seed_ = seed;
    pRNG_mt19937.seed(this->seed_);
}

void Mt19937::init_inverse_maximum() {
    inv_max = 1.0/static_cast<double>(pRNG_mt19937.max());
}

double Mt19937::normal(const double mean, const double std_dev) {
    std::normal_distribution<double> dist(mean, std_dev);
    return dist(pRNG_mt19937);
}

}  // namespace Random
