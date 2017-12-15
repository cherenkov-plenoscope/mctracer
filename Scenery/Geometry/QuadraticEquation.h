// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_QUADRATICEQUATION_H_
#define SCENERY_GEOMETRY_QUADRATICEQUATION_H_

class QuadraticEquation{
    // f(x) = ax^2 + bx + c
    // p = b/a
    // q = c/a
    // x_+ = -p/2 + sqrt( (p/2)^2 - q )
    double p_over_2;
    double q;
    double inner_part_of_squareroot;
    double squareroot;

 public:
    QuadraticEquation(const double _p, const double _q);
    bool has_valid_solutions();
    double minus_solution();
    double plus_solution();
};

#endif  // SCENERY_GEOMETRY_QUADRATICEQUATION_H_
