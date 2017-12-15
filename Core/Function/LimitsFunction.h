// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_LIMITSFUNCTION_H_
#define CORE_FUNCTION_LIMITSFUNCTION_H_

#include <string>

namespace Function {

class Limits{
    double lower_;
    double upper_;

 public:
    static const Limits VOID_LIMITS;
    Limits();
    Limits(const double lower, const double upper);
    void assert_limits_are_causal()const;
    double lower()const;
    double upper()const;
    double range()const;
    void assert_contains(const double x)const;
    std::string str()const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_LIMITSFUNCTION_H_
