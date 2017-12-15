// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_FUNC1DFUNCTION_H_
#define CORE_FUNCTION_FUNC1DFUNCTION_H_

#include "LimitsFunction.h"
#include <iostream>
#include <vector>
#include <string>

namespace Function {

class Func1D {
 protected:
    Func1D();
    Limits limits_;

 public:
    explicit Func1D(const Limits &_limits);
    virtual ~Func1D();
    std::vector<std::vector<double>> sample(const unsigned int N)const;
    double mean(const unsigned int N)const;
    virtual double evaluate(const double x)const = 0;
    Limits get_limits()const;
    virtual std::string str()const;

 protected:
    double increment_for_steps(const unsigned int N)const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_FUNC1DFUNCTION_H_
