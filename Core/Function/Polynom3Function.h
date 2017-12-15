// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_POLYNOM3FUNCTION_H_
#define CORE_FUNCTION_POLYNOM3FUNCTION_H_

#include "Core/Function/Func1DFunction.h"

namespace Function {
class Polynom3 : public Func1D {
    double a3, a2, a1, a0;

 public:
    Polynom3();
    Polynom3(
        const double a3,
        const double a2,
        const double a1,
        const double a0,
        const Limits &limits);
    void init(
        const double a3,
        const double a2,
        const double a1,
        const double a0,
        const Limits &limits);
    double evaluate(const double x)const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_POLYNOM3FUNCTION_H_
