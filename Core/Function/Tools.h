// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_TOOLS_H_
#define CORE_FUNCTION_TOOLS_H_

#include <stdint.h>
#include <vector>
#include "Function.h"

namespace relleums {
namespace function {

static const unsigned int DEFAULT_STEPS = 4096;

double increment_for_steps(
    const Limits &l,
    const uint64_t num_samples);

std::vector<std::vector<double>> sample(
    const Func1 &f,
    const uint64_t num_samples);

double mean(
    const Func1 &f,
    const uint64_t num_samples);

Func1 integral(
    const Func1 &f,
    uint64_t steps = DEFAULT_STEPS);

Func1 inverse(
    const Func1 &f,
    uint64_t steps = DEFAULT_STEPS);

Func1 derivative(
    const Func1 &f,
    uint64_t steps = DEFAULT_STEPS);

bool value_flips_sign(
    const Func1 &f,
    uint64_t steps = DEFAULT_STEPS);

std::vector<std::vector<double>> polynom3(
    double c3,
    double c2,
    double c1,
    double c0,
    double start,
    double end,
    uint64_t steps
);

}  // namespace function
}  // namespace relleums

#endif  // CORE_FUNCTION_TOOLS_H_
