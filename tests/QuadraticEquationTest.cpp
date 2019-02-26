// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/geometry/QuadraticEquation.h"

using namespace merlict;



TEST_CASE("QuadraticEquationTest: no_valid_solution", "[mctracer]") {
  // ax^2 + bx + c = 0
  double b_over_a = 1.0;
  double c_over_a = 1.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  CHECK(!eq.has_valid_solutions());
}

TEST_CASE("QuadraticEquationTest: one_valid_solution", "[mctracer]") {
  // ax^2 + bx + c = 0
  double b_over_a = 0.0;
  double c_over_a = 0.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  CHECK(eq.has_valid_solutions());
  CHECK(eq.minus_solution() == 0.0);
  CHECK(eq.plus_solution() == 0.0);
}

TEST_CASE("QuadraticEquationTest: two_valid_solutions", "[mctracer]") {
  // ax^2 + bx + c = 0
  double b_over_a = 0.0;
  double c_over_a = -1.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  CHECK(eq.has_valid_solutions());
  CHECK(eq.minus_solution() == -1.0);
  CHECK(eq.plus_solution() == +1.0);
}

TEST_CASE("QuadraticEquationTest: vaious_combinations", "[mctracer]") {
  // ax^2 + bx + c = 0

  for (double a = -5.0; a < 5.0; a = a+.25) {
    for (double b = -5.0; b < 5.0; b = b+.25) {
      for (double c = -5.0; c < 5.0; c = c+.25) {
        if (a != 0.0) {
          QuadraticEquation eq(b/a, c/a);
          if (eq.has_valid_solutions()) {
              double xp = eq.plus_solution();
              double xm = eq.minus_solution();
              CHECK(0.0 == Approx(a*xp*xp + b*xp + c).margin(1e-12));
              CHECK(0.0 == Approx(a*xm*xm + b*xm + c).margin(1e-12));
          }
        }
      }
    }
  }
}
