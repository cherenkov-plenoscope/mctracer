// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_LIMITS_H_
#define CORE_FUNCTION_LIMITS_H_

#include <string>

namespace relleums {
namespace Function {

struct Limits{
    double lower;
    double upper;
    void assert_causal()const;
    Limits();
    Limits(const double _lower, const double _upper);
    double range()const;
    void assert_contains(const double x)const;
    std::string str()const;
};

}  // namespace Function
}  // namespace relleums

#endif  // CORE_FUNCTION_LIMITS_H_
