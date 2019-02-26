// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/function/function.h"
using namespace merlict;



// Limits
TEST_CASE("FunctionTest: causal_limits_correct", "[mctracer]") {
    CHECK_NOTHROW(function::Limits(0.0, 1.0));
}

TEST_CASE("FunctionTest: causal_limits_false", "[mctracer]") {
    CHECK_THROWS_AS(function::Limits(0.0, -1.0), std::logic_error);
}

TEST_CASE("FunctionTest: causal_limits_lower_included", "[mctracer]") {
    function::Limits l(0.0, 1.0);
    CHECK_NOTHROW(l.assert_contains(0.0));
}

TEST_CASE("FunctionTest: causal_limits_upper_excluded", "[mctracer]") {
    function::Limits l(0.0, 1.0);
    CHECK_THROWS_AS(l.assert_contains(1.0), std::out_of_range);
}

TEST_CASE("FunctionTest: causal_limits_above_upper", "[mctracer]") {
    function::Limits l(0.0, 1.0);
    CHECK_THROWS_AS(l.assert_contains(1.1), std::out_of_range);
}

TEST_CASE("FunctionTest: causal_limits_below_lower", "[mctracer]") {
    function::Limits l(0.0, 1.0);
    CHECK_THROWS_AS(l.assert_contains(-0.1), std::out_of_range);
}

TEST_CASE("FunctionTest: check_limits", "[mctracer]") {
    function::Limits l(4.2, 133.7);
    CHECK(l.lower == 4.2);
    CHECK(l.upper == 133.7);
}

TEST_CASE("FunctionTest: default_limits", "[mctracer]") {
    function::Limits l;
    CHECK(l.lower == 0.0);
    CHECK(l.upper == 0.0);
    CHECK_THROWS_AS(l.assert_contains(-0.1), std::out_of_range);
    CHECK_THROWS_AS(l.assert_contains(0.0), std::out_of_range);
    CHECK_THROWS_AS(l.assert_contains(+0.1), std::out_of_range);
}

TEST_CASE("FunctionTest: limit_default_range", "[mctracer]") {
    function::Limits l;
    CHECK(l.range() == 0.0);
}

TEST_CASE("FunctionTest: limit_range", "[mctracer]") {
    function::Limits l(0.0, 1.0);
    CHECK(l.range() == 1.0);
}

TEST_CASE("FunctionTest: polynom3_size", "[mctracer]") {
    const uint64_t num_samples = 137;
    std::vector<std::vector<double>> f = function::polynom3(
        0, 0, 1, 0, -1, 1, num_samples);
    REQUIRE(num_samples == f.size());
}

TEST_CASE("FunctionTest: polynom3_limits", "[mctracer]") {
    const uint64_t num_samples = 137;
    std::vector<std::vector<double>> f = function::polynom3(
        0, 0, 1, 0, -1, 1, num_samples);
    REQUIRE(num_samples == f.size());
    REQUIRE(2u == f.at(0).size());
    REQUIRE(-1 == f.at(0).at(0));
    REQUIRE(2u == f.at(num_samples - 1).size());
    REQUIRE(1 == f.at(num_samples - 1).at(0));
}

// Sampling
TEST_CASE("FunctionTest: sampling_table_size", "[mctracer]") {
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, -4.2, 1.337, 4096)); // f(x) = x
    std::vector<std::vector<double>> sample_xy = function::sample(f, 1000);
    REQUIRE(sample_xy.size() == 1000u);
    for (unsigned int i = 0; i < sample_xy.size(); i++)
        REQUIRE(sample_xy.at(i).size() == 2u);
}

TEST_CASE("FunctionTest: sampling_table_x_values", "[mctracer]") {
    const unsigned int samples = 10;
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, -4.2, 1.337, 4096)); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = function::sample(f, samples);
    REQUIRE(sample_xy.size() == samples);
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        REQUIRE(sample_xy.at(i).size() == 2u);
        double x = -4.2 +
            static_cast<double>(i)*(1.337+4.2)/
            static_cast<double>(samples);
        CHECK(x == Approx(sample_xy[i][0]).margin(1.5e-3));
    }
}

TEST_CASE("FunctionTest: sampling_table_y_values", "[mctracer]") {
    const unsigned int samples = 10;
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, -4.2, 1.337, 4096)); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = function::sample(f, samples);
    REQUIRE(sample_xy.size() == samples);
    for (unsigned int i = 0; i < sample_xy.size(); i++) {
        REQUIRE(sample_xy.at(i).size() == 2u);
        double x = sample_xy[i][0];
        CHECK(x*x == Approx(f.evaluate(x)).margin(1e-6));
        CHECK(x*x == Approx(sample_xy[i][1]).margin(1e-6));
    }
}

TEST_CASE("FunctionTest: numerical_integration_const", "[mctracer]") {
    // f(x) = 1
    // F(x) = x
    function::Func1 f(
        function::polynom3(0, 0, 0, 1, 0, 1, 4096));
    function::Func1 F = function::integral(f);
    CHECK(F.limits.lower == f.limits.lower);
    CHECK(F.limits.upper == f.limits.upper);
    // F(0) != 0
    CHECK(0.0 == Approx(F.evaluate(0.0)).margin(1e-3));
    // F(0.5) != 0.5
    CHECK(0.5 == Approx(F.evaluate(0.5)).margin(1e-3));
    // F(1) != 1
    CHECK(1.0 == Approx(F.evaluate(1.0-1e-6)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_integration_linear", "[mctracer]") {
    // f(x) = x
    // F(x) = 1/2 x^2
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    function::Func1 F = function::integral(f);
    CHECK(F.limits.lower == f.limits.lower);
    CHECK(F.limits.upper == f.limits.upper);
    // F(0) != 0
    CHECK(0.0 == Approx(F.evaluate(0.0)).margin(1e-3));
    // F(0.5) != 0.5
    CHECK(0.125 == Approx(F.evaluate(0.5)).margin(1e-3));
    // F(1) != 1
    CHECK(0.5 == Approx(F.evaluate(1.0-1e-6)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_inverse_limits", "[mctracer]") {
    // f(x) = 2x [0, 1)
    // f_inv(x) = 1/2x [0, 2)
    function::Func1 f(
        function::polynom3(0, 0, 2, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    CHECK(0.0 == Approx(f_inv.limits.lower).margin(1e-6));
    CHECK(2.0 == Approx(f_inv.limits.upper).margin(1e-6));
}

TEST_CASE("FunctionTest: numerical_inverse_linear", "[mctracer]") {
    // f(x) = x
    // f_inv(x) = x
    function::Func1 f(
        function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    CHECK(0.0 == Approx(f_inv.limits.lower).margin(1e-6));
    CHECK(1.0 == Approx(f_inv.limits.upper).margin(1e-6));
    // f_inv(0) != 0
    CHECK(0.0 == Approx(f_inv.evaluate(0.0)).margin(1e-3));
    // f_inv(0.5) != 0.5
    CHECK(0.5 == Approx(f_inv.evaluate(0.5)).margin(1e-3));
    // f_inv(1) != 1
    CHECK(1.0 == Approx(f_inv.evaluate(1.0-1e-6)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_inverse_quadratic", "[mctracer]") {
    // f(x) = x^2
    // f_inv(x) = sqrt(x)
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    function::Func1 f_inv = function::inverse(f);
    CHECK(0.0 == Approx(f_inv.limits.lower).margin(1e-6));
    CHECK(1.0 == Approx(f_inv.limits.upper).margin(1e-6));
    // tsvio::write_table_to_file(f.sample(1024), "f.func");
    // tsvio::write_table_to_file(f_inv.sample(1024), "f_inv.func");
    // f_inv(0) != sqrt(0) = 0
    CHECK(0.0 == Approx(f_inv.evaluate(0.0)).margin(1e-3));
    // f_inv(0.5) != sqrt(0.5) = sqrt(2)/2
    CHECK(sqrt(2.0)/2.0 == Approx(f_inv.evaluate(0.5)).margin(1e-3));
    // f_inv(1) != sqrt(1.0) = 1.0
    CHECK(1.0 == Approx(f_inv.evaluate(1.0-1e-6)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_derivative_of_constant_function", "[mctracer]") {
    // f(x) = 1
    // f'(x) = 0
    function::Func1 f(
        function::polynom3(0, 0, 0, 1, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    CHECK(f_prime.limits.lower == f.limits.lower);
    CHECK(f_prime.limits.upper == f.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        CHECK(0.0 == Approx(f_prime.evaluate(x)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_derivative_of_linear_function", "[mctracer]") {
    // f(x) = x + 1
    // f'(x) = 1
    function::Func1 f(
        function::polynom3(0, 0, 1, 1, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    CHECK(f_prime.limits.lower == f.limits.lower);
    CHECK(f_prime.limits.upper == f.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        CHECK(1.0 == Approx(f_prime.evaluate(x)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_derivative_of_quadratic_function", "[mctracer]") {
    // f(x) = x^2
    // f'(x) = 2*x
    function::Func1 f(
        function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    function::Func1 f_prime = function::derivative(f);
    CHECK(f_prime.limits.lower == f.limits.lower);
    CHECK(f_prime.limits.upper == f.limits.upper);
    for (double x = 0; x < 1.0; x = x+1e-3)
        CHECK(2.0*x == Approx(f_prime.evaluate(x)).margin(1e-3));
}

TEST_CASE("FunctionTest: numerical_sign_flip_of_value_of_constant_function", "[mctracer]") {
    // f(x) = 0
    // sign flip -> no
    function::Func1 f(
        function::polynom3(0, 0, 0, 0, 0, 1, 4096));
    CHECK(!function::value_flips_sign(f));
}

TEST_CASE("FunctionTest: numerical_sign_flip_of_value_of_linear_function", "[mctracer]") {
    // f(x) = x [-1, 1)
    // sign flip -> yes
    function::Func1 f(function::polynom3(0, 0, 1, 0, -1, 1, 4096));
    CHECK(function::value_flips_sign(f));
}

TEST_CASE("FunctionTest: numerical_sign_flip_of_value_of_quadratic_function", "[mctracer]") {
    // f(x) = x^2 - 1 [-1, 1)
    // sign flip -> yes
    function::Func1 f(function::polynom3(0, 1, 0, -1, -1, 1, 4096));
    CHECK(function::value_flips_sign(f));
}

TEST_CASE("FunctionTest: numerical_sign_flip_of_value_of_quadratic_function_no", "[mctracer]") {
    // f(x) = x^2 [0, 1)
    // sign flip -> no
    function::Func1 f(function::polynom3(0, 1, 0, 0, 0, 1, 4096));
    CHECK(!function::value_flips_sign(f));
}


TEST_CASE("FunctionTest: mean", "[mctracer]") {
    // f(x) = x [0, 1)
    // sign flip -> no
    function::Func1 f(function::polynom3(0, 0, 1, 0, 0, 1, 4096));
    const double mean = function::mean(f, 1337);
    CHECK(mean == Approx(0.5).margin(1e-3));
}
