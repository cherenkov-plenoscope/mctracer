// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_LIMITS_H_
#define CORE_FUNCTION_LIMITS_H_

#include <string>

namespace merlict {
namespace function {

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

}  // namespace function
}  // namespace relleums

#endif  // CORE_FUNCTION_LIMITS_H_
