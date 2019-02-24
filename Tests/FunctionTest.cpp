// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "gtest/gtest.h"
#include "Core/function/function.h"
using namespace relleums;

class FunctionTest : public ::testing::Test {};

// Limits
TEST_F(FunctionTest, causal_limits_correct) {
    EXPECT_NO_THROW(function::Limits l(0.0, 1.0));
}

TEST_F(FunctionTest, causal_limits_false) {
    EXPECT_THROW(function::Limits l(0.0, -1.0), std::logic_error);
}

TEST_F(FunctionTest, causal_limits_lower_included) {
    function::Limits l(0.0, 1.0);
    EXPECT_NO_THROW(l.assert_contains(0.0));
}

TEST_F(FunctionTest, causal_limits_upper_excluded) {
    function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.0), std::out_of_range);
}

TEST_F(FunctionTest, causal_limits_above_upper) {
    function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.1), std::out_of_range);
}

TEST_F(FunctionTest, causal_limits_below_lower) {
    function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(-0.1), std::out_of_range);
}

TEST_F(FunctionTest, check_limits) {
    function::Limits l(4.2, 133.7);
    EXPECT_EQ(4.2, l.lower);
    EXPECT_EQ(133.7, l.upper);
}

TEST_F(FunctionTest, default_limits) {
    function::Limits l;
    EXPECT_EQ(0.0, l.lower);
    EXPECT_EQ(0.0, l.upper);
    EXPECT_THROW(l.assert_contains(-0.1), std::out_of_range);
    EXPECT_THROW(l.assert_contains(0.0), std::out_of_range);
    EXPECT_THROW(l.assert_contains(+0.1), std::out_of_range);
}

TEST_F(FunctionTest, limit_default_range) {
    function::Limits l;
    EXPECT_EQ(0.0, l.range());
}

TEST_F(FunctionTest, limit_range) {
    function::Limits l(0.0, 1.0);
    EXPECT_EQ(1.0, l.range());
}

TEST_F(FunctionTest, polynom3_size) {
    const uint64_t num_samples = 137;
    std::vector<std::vector<double>> f = function::polynom3(
        0, 0, 1, 0, -1, 1, num_samples);
    ASSERT_EQ(f.size(), num_samples);
}

TEST_F(FunctionTest, polynom3_limits) {
    const uint64_t num_samples = 137;
    std::vector<std::vector<double>> f = function::polynom3(
        0, 0, 1, 0, -1, 1, num_samples);
    ASSERT_EQ(f.size(), num_samples);
    ASSERT_EQ(f.at(0).size(), 2u);
    ASSERT_EQ(f.at(0).at(0), -1);
    ASSERT_EQ(f.at(num_samples - 1).size(), 2u);
    ASSERT_EQ(f.at(num_samples - 1).at(0), 1);
}

// Sampling
TEST_F(FunctionTest, sampling_table_size) {
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, -4.2, 1.337, 4096)); // f(x) = x
    std::vector<std::vector<double>> sample_xy = function::sample(f, 1000);
    ASSERT_EQ(1000u, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++)
        ASSERT_EQ(2u, sample_xy.at(i).size());
}

TEST_F(FunctionTest, sampling_table_x_values) {
    const unsigned int samples = 10;
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, -4.2, 1.337, 4096)); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = function::sample(f, samples);
    ASSERT_EQ(samples, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        ASSERT_EQ(2u, sample_xy.at(i).size());
        double x = -4.2 +
            static_cast<double>(i)*(1.337+4.2)/
            static_cast<double>(samples);
        EXPECT_NEAR(x, sample_xy[i][0], 1.5e-3);
    }
}

TEST_F(FunctionTest, sampling_table_y_values) {
    const unsigned int samples = 10;
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, -4.2, 1.337, 4096)); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = function::sample(f, samples);
    ASSERT_EQ(samples, sample_xy.size());
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        ASSERT_EQ(2u, sample_xy.at(i).size());
        double x = sample_xy[i][0];
        EXPECT_NEAR(x*x, f.evaluate(x), 1e-6);
        EXPECT_NEAR(x*x, sample_xy[i][1], 1e-6);
    }
}

TEST_F(FunctionTest, numerical_integration_const) {
    // f(x) = 1
    // F(x) = x
    function::Func1 f(
        function::polynom3(0, 0, 0, 1, 0, 1, 4096));
    function::Func1 F = function::integral(f);
    EXPECT_EQ(f.limits.lower, F.limits.lower);
    EXPECT_EQ(f.limits.upper, F.limits.upper);
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
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    function::Func1 F = function::integral(f);
    EXPECT_EQ(f.limits.lower, F.limits.lower);
    EXPECT_EQ(f.limits.upper, F.limits.upper);
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
    function::Func1 f(
        function::polynom3(0, 0, 2, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits.lower, 1e-6);
    EXPECT_NEAR(2.0, f_inv.limits.upper, 1e-6);
}

TEST_F(FunctionTest, numerical_inverse_linear) {
    // f(x) = x
    // f_inv(x) = x
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits.lower, 1e-6);
    EXPECT_NEAR(1.0, f_inv.limits.upper, 1e-6);
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
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    EXPECT_NEAR(0.0, f_inv.limits.lower, 1e-6);
    EXPECT_NEAR(1.0, f_inv.limits.upper, 1e-6);
    // tsvio::write_table_to_file(f.sample(1024), "f.func");
    // tsvio::write_table_to_file(f_inv.sample(1024), "f_inv.func");
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
    function::Func1 f(
        function::polynom3(0, 0, 0, 1, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    EXPECT_EQ(f.limits.lower, f_prime.limits.lower);
    EXPECT_EQ(f.limits.upper, f_prime.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(0.0, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_derivative_of_linear_function) {
    // f(x) = x + 1
    // f'(x) = 1
    function::Func1 f(
        function::polynom3(0, 0, 1, 1, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    EXPECT_EQ(f.limits.lower, f_prime.limits.lower);
    EXPECT_EQ(f.limits.upper, f_prime.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(1.0, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_derivative_of_quadratic_function) {
    // f(x) = x^2
    // f'(x) = 2*x
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    EXPECT_EQ(f.limits.lower, f_prime.limits.lower);
    EXPECT_EQ(f.limits.upper, f_prime.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        EXPECT_NEAR(2.0*x, f_prime.evaluate(x), 1e-3);
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_constant_function) {
    // f(x) = 0
    // sign flip -> no
    function::Func1 f(
        function::polynom3(0, 0, 0, 0, 0, 1, 4096));
    EXPECT_FALSE(function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_linear_function) {
    // f(x) = x [-1, 1)
    // sign flip -> yes
    function::Func1 f(function::polynom3(0, 0, 1, 0, -1, 1, 4096));
    EXPECT_TRUE(function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_quadratic_function) {
    // f(x) = x^2 - 1 [-1, 1)
    // sign flip -> yes
    function::Func1 f(function::polynom3(0, 1, 0, -1, -1, 1, 4096));
    EXPECT_TRUE(function::value_flips_sign(f));
}

TEST_F(FunctionTest, numerical_sign_flip_of_value_of_quadratic_function_no) {
    // f(x) = x^2 [0, 1)
    // sign flip -> no
    function::Func1 f(function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    EXPECT_FALSE(function::value_flips_sign(f));
}


TEST_F(FunctionTest, mean) {
    // f(x) = x [0, 1)
    // sign flip -> no
    function::Func1 f(function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    const double mean = function::mean(f, 1337);
    EXPECT_NEAR(mean, 0.5, 1e-3);
}
