// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/LimitsFunction.h"
#include <sstream>
#include <exception>

namespace Function {

const Limits Limits::VOID_LIMITS = Limits(0.0, 0.0);

Limits::Limits(const double lower, const double upper) {
    lower_ = lower;
    upper_ = upper;
    assert_limits_are_causal();
}

Limits::Limits() {
    lower_ = 0.0;
    upper_ = 0.0;
    assert_limits_are_causal();
}

void Limits::assert_limits_are_causal()const {
    if (upper_ < lower_) {
        std::stringstream info;
        info << "Numerical Function Limit in file " << __FILE__ << "\n";
        info << "Expected upper limit u to be ";
        info << "equal or larger the lower limit l, but actual ";
        info << "u=" << upper_ << " , l=" << lower_ << "\n";
        throw std::logic_error(info.str());
    }
}

double Limits::get_lower()const {
    return lower_;
}

double Limits::get_upper()const {
    return upper_;
}

double Limits::get_range()const {
    return upper_ - lower_;
}

void Limits::assert_contains(const double x)const {
    if (x < lower_ || x >= upper_) {
        std::stringstream info;
        info << "Numerical Function 1D in file " << __FILE__ << "\n";
        info << "Expected function argument x ";
        info << "to be in limits " << lower_ << "<= x <";
        info << upper_ << " but actual x=" << x << "\n";
        throw std::out_of_range(info.str());
    }
}

std::string Limits::str()const {
    std::stringstream out;
    out << "[" << lower_ << ", " << upper_ << ")";
    return out.str();
}

}  // namespace Function
