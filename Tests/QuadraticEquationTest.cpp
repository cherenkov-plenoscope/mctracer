// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/Geometry/QuadraticEquation.h"

class QuadraticEquationTest : public ::testing::Test {};

TEST_F(QuadraticEquationTest, no_valid_solution) {
  // ax^2 + bx + c = 0
  double b_over_a = 1.0;
  double c_over_a = 1.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  EXPECT_FALSE(eq.has_valid_solutions());
}

TEST_F(QuadraticEquationTest, one_valid_solution) {
  // ax^2 + bx + c = 0
  double b_over_a = 0.0;
  double c_over_a = 0.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  EXPECT_TRUE(eq.has_valid_solutions());
  EXPECT_EQ(0.0, eq.minus_solution());
  EXPECT_EQ(0.0, eq.plus_solution());
}

TEST_F(QuadraticEquationTest, two_valid_solutions) {
  // ax^2 + bx + c = 0
  double b_over_a = 0.0;
  double c_over_a = -1.0;
  QuadraticEquation eq(b_over_a, c_over_a);

  EXPECT_TRUE(eq.has_valid_solutions());
  EXPECT_EQ(-1.0, eq.minus_solution());
  EXPECT_EQ(+1.0, eq.plus_solution());
}

TEST_F(QuadraticEquationTest, vaious_combinations) {
  // ax^2 + bx + c = 0

  for (double a = -5.0; a < 5.0; a = a+.25) {
    for (double b = -5.0; b < 5.0; b = b+.25) {
      for (double c = -5.0; c < 5.0; c = c+.25) {
        if (a != 0.0) {
          QuadraticEquation eq(b/a, c/a);
          if (eq.has_valid_solutions()) {
              double xp = eq.plus_solution();
              double xm = eq.minus_solution();
              EXPECT_NEAR(0.0, a*xp*xp + b*xp + c, 1e-12);
              EXPECT_NEAR(0.0, a*xm*xm + b*xm + c, 1e-12);
          }
        }
      }
    }
  }
}
