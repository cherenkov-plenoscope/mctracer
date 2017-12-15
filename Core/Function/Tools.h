// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_TOOLS_H_
#define CORE_FUNCTION_TOOLS_H_

#include "Function.h"

namespace Function {
    static const unsigned int DEFAULT_STEPS = 4096;
    LinInterpol get_integral(
        const Func1D &f,
        unsigned int steps = DEFAULT_STEPS);
    LinInterpol get_inverse(
        const Func1D &f,
        unsigned int steps = DEFAULT_STEPS);
    LinInterpol get_derivative(
        const Func1D &f,
        unsigned int steps = DEFAULT_STEPS);
    bool value_flips_sign(
        const Func1D &f,
        unsigned int steps = DEFAULT_STEPS);

}  // namespace Function

#endif  // CORE_FUNCTION_TOOLS_H_
