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
    Limits limits;

 public:
    Func1D() {}
    explicit Func1D(const Limits &_limits);
    void set_limits(const Limits limits);
    virtual ~Func1D();
    std::vector<std::vector<double>> get_samples(const unsigned int N)const;
    double get_mean(const unsigned int N)const;
    virtual double operator()(const double x)const = 0;
    Limits get_limits()const;
    virtual std::string str()const;

 protected:
    double increment_for_steps(const unsigned int N)const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_FUNC1DFUNCTION_H_
