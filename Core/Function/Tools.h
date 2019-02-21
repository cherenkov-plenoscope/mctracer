// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_TOOLS_H_
#define CORE_FUNCTION_TOOLS_H_

#include <stdint.h>
#include <vector>
#include "Function.h"

namespace relleums {
namespace Function {

static const unsigned int DEFAULT_STEPS = 4096;
LinInterpol get_integral(const Func1D &f, unsigned int steps = DEFAULT_STEPS);
LinInterpol get_inverse(const Func1D &f, unsigned int steps = DEFAULT_STEPS);
LinInterpol get_derivative(const Func1D &f, unsigned int steps = DEFAULT_STEPS);
bool value_flips_sign(const Func1D &f, unsigned int steps = DEFAULT_STEPS);
std::vector<std::vector<double>> polynom3(
    double c3,
    double c2,
    double c1,
    double c0,
    double start,
    double end,
    uint64_t steps
);

}  // namespace Function
}  // namespace relleums

#endif  // CORE_FUNCTION_TOOLS_H_
