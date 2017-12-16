// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "gtest/gtest.h"
#include "Core/Function/Function.h"

class FunctionTest : public ::testing::Test {};

// Limits
TEST_F(FunctionTest, causal_limits_correct) {
    EXPECT_NO_THROW(Function::Limits l(0.0, 1.0));
}

TEST_F(FunctionTest, causal_limits_false) {
    EXPECT_THROW(Function::Limits l(0.0, -1.0), std::logic_error);
}

TEST_F(FunctionTest, causal_limits_lower_included) {
    Function::Limits l(0.0, 1.0);
    EXPECT_NO_THROW(l.assert_contains(0.0));
}

TEST_F(FunctionTest, causal_limits_upper_excluded) {
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.0), std::out_of_range);
}

TEST_F(FunctionTest, causal_limits_above_upper) {
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.1), std::out_of_range);
}

TEST_F(FunctionTest, causal_limits_below_lower) {
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(-0.1), std::out_of_range);
}

TEST_F(FunctionTest, check_limits) {
    Function::Limits l(4.2, 133.7);
    EXPECT_EQ(4.2, l.lower());
    EXPECT_EQ(133.7, l.upper());
}

TEST_F(FunctionTest, default_limits) {
    Function::Limits l;
    EXPECT_EQ(0.0, l.lower());
    EXPECT_EQ(0.0, l.upper());
    EXPECT_THROW(l.assert_contains(-0.1), std::out_of_range);
    EXPECT_THROW(l.assert_contains(0.0), std::out_of_range);
    EXPECT_THROW(l.assert_contains(+0.1), std::out_of_range);
}

TEST_F(FunctionTest, limit_default_range) {
    Function::Limits l;
    EXPECT_EQ(0.0, l.range());
}

TEST_F(FunctionTest, limit_range) {
    Function::Limits l(0.0, 1.0);
    EXPECT_EQ(1.0, l.range());
}

// Constant
TEST_F(FunctionTest, ConstantFunction) {
    Function::Limits l(-100.0, 100.0);
    double y = 133.7;
    Function::Constant f(y, l);
    for (double x = -1e2; x < 1e2; x = x+0.55)
        EXPECT_EQ(y, f.evaluate(x));
}

TEST_F(FunctionTest, constant_function_limits) {
    Function::Limits l(0.0, 1.0);
    double y = 133.7;
    Function::Constant f(y, l);
    EXPECT_THROW(f.evaluate(-0.5), std::out_of_range);
    EXPECT_NO_THROW(f.evaluate(0.0));
    EXPECT_NO_THROW(f.evaluate(0.5));
    EXPECT_THROW(f.evaluate(1.0), std::out_of_range);
    EXPECT_THROW(f.evaluate(1.5), std::out_of_range);
}

// Polynom3
TEST_F(FunctionTest, Polynom3) {
    Function::Limits l(0.0, 1.0);
    Function::Polynom3 f(1.0, 1.0, 1.0, 1.0, l);
    for (double x = -0.0; x < 1.0; x = x+0.11)
        EXPECT_EQ(x*x*x + x*x + x + 1.0, f.evaluate(x));
}

TEST_F(FunctionTest, Polynom3_limits) {
    Function::Limits l(0.0, 1.0);
    Function::Polynom3 f(1.0, 1.0, 1.0, 1.0, l);
    EXPECT_THROW(f.evaluate(-0.5), std::out_of_range);
    EXPECT_NO_THROW(f.evaluate(0.0));
    EXPECT_NO_THROW(f.evaluate(0.5));
    EXPECT_THROW(f.evaluate(1.0), std::out_of_range);
    EXPECT_THROW(f.evaluate(1.5), std::out_of_range);
}

// Sampling
TEST_F(FunctionTest, sampling_table_size) {
    Function::Limits l(-4.2, 1.337);
    Function::Polynom3 f(0.0, 0.0, 1.0, 0.0, l);  // f(x) = x
    std::vector<std::vector<double>> sample_xy = f.sample(1000);
    ASSERT_EQ(1000u, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++)
        ASSERT_EQ(2u, sample_xy.at(i).size());
}

TEST_F(FunctionTest, sampling_table_x_values) {
    const unsigned int samples = 10;
    Function::Limits l(-4.2, 1.337);
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, l);  // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = f.sample(samples);
    ASSERT_EQ(samples, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        ASSERT_EQ(2u, sample_xy.at(i).size());
        double x = -4.2 +
            static_cast<double>(i)*(1.337+4.2)/
            static_cast<double>(samples);
        EXPECT_NEAR(x, sample_xy[i][0], 1e-9);
    }
}

TEST_F(FunctionTest, sampling_table_y_values) {
    const unsigned int samples = 10;
    Function::Limits l(-4.2, 1.337);
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, l);  // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = f.sample(samples);
    ASSERT_EQ(samples, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        ASSERT_EQ(2u, sample_xy.at(i).size());
        double x = sample_xy[i][0];
        EXPECT_NEAR(x*x, f.evaluate(x), 1e-9);
        EXPECT_NEAR(x*x, sample_xy[i][1], 1e-9);
    }
}

// Concat
TEST_F(FunctionTest, Concat_valid_limits) {
    Function::Limits l1(0.0, 1.0);
    Function::Constant f1(1.0, l1);
    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);
    Function::Limits l3(2.0, 3.0);
    Function::Constant f3(1.0, l3);
    std::vector<Function::Func1D*> f = {&f1, &f2, &f3};
    Function::Concat con(f);
    EXPECT_EQ(0.0, con.limits().lower());
    EXPECT_EQ(3.0, con.limits().upper());
}

TEST_F(FunctionTest, Concat_bad_limits) {
    Function::Limits l1(0.0, 0.9);
    Function::Polynom3 f1(1.0, 1.0, 1.0, 1.0, l1);
    Function::Limits l2(1.1, 2.0);
    Function::Constant f2(4.2, l2);
    std::vector<Function::Func1D*> f = {&f1, &f2};
    EXPECT_THROW(Function::Concat con(f), std::logic_error);
}

TEST_F(FunctionTest, Concat_bad_access) {
    Function::Limits l1(0.0, 1.0);
    Function::Polynom3 f1(1.0, 1.0, 1.0, 1.0, l1);
    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);
    std::vector<Function::Func1D*> f = {&f1, &f2};
    Function::Concat con(f);
    EXPECT_THROW(con.evaluate(-0.5), std::out_of_range);
    EXPECT_NO_THROW(con.evaluate(0.0));
    EXPECT_NO_THROW(con.evaluate(0.5));
    EXPECT_NO_THROW(con.evaluate(1.0));
    EXPECT_NO_THROW(con.evaluate(1.5));
    EXPECT_THROW(con.evaluate(2.0), std::out_of_range);
    EXPECT_THROW(con.evaluate(2.5), std::out_of_range);
}

TEST_F(FunctionTest, Concat_y_in_different_sub_functions) {
    Function::Limits l1(0.0, 1.0);
    Function::Constant f1(1.337, l1);
    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);
    Function::Limits l3(2.0, 3.0);
    Function::Constant f3(3.141, l3);
    std::vector<Function::Func1D*> f = {&f1, &f2, &f3};
    Function::Concat con(f);
    for (double x = -0.5; x < 3.5; x = x+0.011) {
        if (x >= 0.0 && x < 1.0)
            EXPECT_EQ(1.337, con.evaluate(x));
        else if (x >= 1.0 && x < 2.0)
            EXPECT_EQ(4.2, con.evaluate(x));
        else if (x >= 2.0 && x < 3.0)
            EXPECT_EQ(3.141, con.evaluate(x));
        else
            EXPECT_THROW(con.evaluate(x), std::out_of_range);
    }
}

TEST_F(FunctionTest, numerical_integration_const) {
    // f(x) = 1
    // F(x) = x
    Function::Polynom3 f(0.0, 0.0, 0.0, 1.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol F = Function::get_integral(f);
    EXPECT_EQ(f.limits().lower(), F.limits().lower());
    EXPECT_EQ(f.limits().upper(), F.limits().upper());
    // F(0) != 0
    EXPECT_NEAR(0.0, F.evaluate(0.0), 1e-3);
    // F(0.5) != 0.5
    EXPECT_NEAR(0.5, F.evaluate(0.5), 1e-3);
    // F(1) != 1
    EXPECT_NEAR(1.0, F.evaluate(1.0-1e-6), 1e-3);
}

TEST_F(FunctionTest, numerical_integration_linear) {
    // f(x) = x
    // F(x) = 1/2 x^2
    Function::Polynom3 f(0.0, 0.0, 1.0, 0.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol F = Function::get_integral(f);
    EXPECT_EQ(f.limits().lower(), F.limits().lower());
    EXPECT_EQ(f.limits().upper(), F.limits().upper());
    // F(0) != 0
    EXPECT_NEAR(0.0, F.evaluate(0.0), 1e-3);
    // F(0.5) != 0.5
    EXPECT_NEAR(0.125, F.evaluate(0.5), 1e-3);
    // F(1) != 1
    EXPECT_NEAR(0.5, F.evaluate(1.0-1e-6), 1e-3);
}

TEST_F(FunctionTest, numerical_inverse_limits) {
    // f(x) = 2x [0, 1)
    // f_inv(x) = 1/2x [0, 2)
    Function::Polynom3 f(0.0, 0.0, 2.0, 0.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_inv = Function::get_inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits().lower(), 1e-6);
    EXPECT_NEAR(2.0, f_inv.limits().upper(), 1e-6);
}

TEST_F(FunctionTest, numerical_inverse_linear) {
    // f(x) = x
    // f_inv(x) = x
    Function::Polynom3 f(0.0, 0.0, 1.0, 0.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_inv = Function::get_inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits().lower(), 1e-6);
    EXPECT_NEAR(1.0, f_inv.limits().upper(), 1e-6);
    // f_inv(0) != 0
    EXPECT_NEAR(0.0, f_inv.evaluate(0.0), 1e-3);
    // f_inv(0.5) != 0.5
    EXPECT_NEAR(0.5, f_inv.evaluate(0.5), 1e-3);
    // f_inv(1) != 1
    EXPECT_NEAR(1.0, f_inv.evaluate(1.0-1e-6), 1e-3);
}

TEST_F(FunctionTest, numerical_inverse_quadratic) {
    // f(x) = x^2
    // f_inv(x) = sqrt(x)
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_inv = Function::get_inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits().lower(), 1e-6);
    EXPECT_NEAR(1.0, f_inv.limits().upper(), 1e-6);
    // AsciiIo::write_table_to_file(f.sample(1024), "f.func");
    // AsciiIo::write_table_to_file(f_inv.sample(1024), "f_inv.func");
    // f_inv(0) != sqrt(0) = 0
    EXPECT_NEAR(0.0, f_inv.evaluate(0.0), 1e-3);
    // f_inv(0.5) != sqrt(0.5) = sqrt(2)/2
    EXPECT_NEAR(sqrt(2.0)/2.0, f_inv.evaluate(0.5), 1e-3);
    // f_inv(1) != sqrt(1.0) = 1.0
    EXPECT_NEAR(1.0, f_inv.evaluate(1.0-1e-6), 1e-3);
}

TEST_F(FunctionTest, numerical_derivative_of_constant_function) {
    // f(x) = 1
    // f'(x) = 0
    Function::Polynom3 f(0.0, 0.0, 0.0, 1.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_prime = Function::get_derivative(f);
    EXPECT_EQ(f.limits().lower(), f_prime.limits().lower());
    EXPECT_EQ(f.limits().upper(), f_prime.limits().upper());
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(0.0, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_derivative_of_linear_function) {
    // f(x) = x + 1
    // f'(x) = 1
    Function::Polynom3 f(0.0, 0.0, 1.0, 1.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_prime = Function::get_derivative(f);
    EXPECT_EQ(f.limits().lower(), f_prime.limits().lower());
    EXPECT_EQ(f.limits().upper(), f_prime.limits().upper());
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(1.0, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_derivative_of_quadratic_function) {
    // f(x) = x^2
    // f'(x) = 2*x
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, Function::Limits(0.0, 1.0));
    Function::LinInterpol f_prime = Function::get_derivative(f);
    EXPECT_EQ(f.limits().lower(), f_prime.limits().lower());
    EXPECT_EQ(f.limits().upper(), f_prime.limits().upper());
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(2.0*x, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_constant_function) {
    // f(x) = 0
    // sign flip -> no
    Function::Polynom3 f(0.0, 0.0, 0.0, 0.0, Function::Limits(0.0, 1.0));
    EXPECT_FALSE(Function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_linear_function) {
    // f(x) = x [-1, 1)
    // sign flip -> yes
    Function::Polynom3 f(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
    EXPECT_TRUE(Function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_quadratic_function) {
    // f(x) = x^2 - 1 [-1, 1)
    // sign flip -> yes
    Function::Polynom3 f(0.0, 1.0, 0.0, -1.0, Function::Limits(-1.0, 1.0));
    EXPECT_TRUE(Function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_quadratic_function_no) {
    // f(x) = x^2 [0, 1)
    // sign flip -> no
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, Function::Limits(0.0, 1.0));
    EXPECT_FALSE(Function::value_flips_sign(f));
}
