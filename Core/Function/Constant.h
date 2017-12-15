// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_CONSTANT_H_
#define CORE_FUNCTION_CONSTANT_H_

#include "Func1D.h"

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
#endif  // CORE_FUNCTION_CONSTANT_H_
