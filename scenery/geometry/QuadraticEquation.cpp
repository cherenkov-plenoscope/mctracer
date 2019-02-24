// Copyright 2014 Sebastian A. Mueller
#include "scenery/geometry/QuadraticEquation.h"
#include <math.h>

namespace relleums {

// y = a*x^2 + b*x + c
// p = b/a
// q = c/a
// x_m = -p/2 - sqrt((-p/2)^2 - q)
// x_p = -p/2 + sqrt((-p/2)^2 - q)
QuadraticEquation::QuadraticEquation(const double _p, const double _q):
    p_over_2(0.5*_p),
    q(_q),
    inner_part_of_squareroot(p_over_2*p_over_2 - q) {
    if (has_valid_solutions())
        squareroot = sqrt(inner_part_of_squareroot);
}

bool QuadraticEquation::has_valid_solutions() {
    return inner_part_of_squareroot >= 0.0;
}

double QuadraticEquation::minus_solution() {
    return -p_over_2 - squareroot;
}

double QuadraticEquation::plus_solution() {
    return -p_over_2 + squareroot;
}

}  // namespace relleums
