// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_LIMITSFUNCTION_H_
#define CORE_FUNCTION_LIMITSFUNCTION_H_

#include <string>

namespace Function {

class Limits{
    double lower;
    double upper;

 public:
    static const Limits VOID_LIMITS;
    Limits();
    Limits(const double _lower, const double _upper);
    void assert_limits_are_causal()const;
    double get_lower()const;
    double get_upper()const;
    double get_range()const;
    void assert_contains(const double x)const;
    std::string str()const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_LIMITSFUNCTION_H_
