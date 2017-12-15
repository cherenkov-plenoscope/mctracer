// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_CONSTANTFUNCTION_H_
#define CORE_FUNCTION_CONSTANTFUNCTION_H_

#include "Func1DFunction.h"

namespace Function {

class Constant :public Func1D {
    double value;

 public:
    Constant();
    Constant(const double value, const Limits &limits);
    void init(const double value, const Limits &limits);
    double evaluate(const double x)const;
    static const Constant VOID_FUNCTION;
};

}  // namespace Function
#endif  // CORE_FUNCTION_CONSTANTFUNCTION_H_
