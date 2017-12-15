// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/ConstantFunction.h"
#include <sstream>

namespace Function {

const Constant Constant::VOID_FUNCTION = Constant(0.0, Limits::VOID_LIMITS);

Constant::Constant(): value(0.0) {}

Constant::Constant(const double value, const Limits &limits) {
    init(value, limits);
}

void Constant::init(const double value, const Limits &limits) {
    this->value = value;
    this->limits_ = limits;
}

double Constant::evaluate(const double x)const {
    limits_.assert_contains(x);
    return value;
}

}  // namespace Function
