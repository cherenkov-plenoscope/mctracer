// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_LIMITS_H_
#define CORE_FUNCTION_LIMITS_H_

#include <string>
#include <sstream>
#include <exception>

namespace relleums {
namespace Function {

struct Limits{
    double lower;
    double upper;

    void assert_causal()const {
        if (upper < lower) {
            std::stringstream info;
            info << "Numerical Function Limit in file " << __FILE__ << "\n";
            info << "Expected upper limit u to be ";
            info << "equal or larger the lower limit l, but actual ";
            info << "u=" << upper << " , l=" << lower << "\n";
            throw std::logic_error(info.str());}}

    Limits(const double _lower, const double _upper):
        lower(_lower), upper(_upper) { assert_causal(); }

    Limits(): lower(0.0), upper(0.0) { assert_causal(); }

    double range()const { return upper - lower; }

    void assert_contains(const double x)const {
        if (x < lower || x >= upper) {
            std::stringstream info;
            info << "Numerical Function 1D in file " << __FILE__ << "\n";
            info << "Expected function argument x ";
            info << "to be in limits " << lower << "<= x <";
            info << upper << " but actual x=" << x << "\n";
            throw std::out_of_range(info.str());}}

    std::string str()const {
        std::stringstream out;
        out << "[" << lower << ", " << upper << ")";
        return out.str();}
};

}  // namespace Function
}  // namespace relleums

#endif  // CORE_FUNCTION_LIMITS_H_
