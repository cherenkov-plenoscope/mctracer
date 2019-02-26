// Copyright 2014 Sebastian A. Mueller
#include "merlict/function/function.h"
#include <math.h>
#include <vector>
using std::vector;

namespace relleums {
namespace function {

double increment_for_steps(const Limits &l, const uint64_t num_samples) {
    return l.range()/static_cast<double>(num_samples);
}

vector<vector<double>> sample(const Func1 &f, const uint64_t num_samples) {
    double arg = f.limits.lower;
    const double increment = increment_for_steps(f.limits, num_samples);
    vector<vector<double>> table;
    table.reserve(num_samples);
    for (uint64_t i = 0; i < num_samples; i++) {
        const double value = f.evaluate(arg);
        table.push_back({arg, value});
        arg = arg + increment;
    }
    return table;
}

double mean(const Func1 &f, const uint64_t num_samples) {
    vector<vector<double>> x_vs_y = sample(f, num_samples);
    double y_mean = 0.0;
    for (vector<double> point : x_vs_y)
        y_mean = y_mean + point.at(1);
    return y_mean/x_vs_y.size();
}

Func1 integral(const Func1 &f, uint64_t steps) {
    double step = f.limits.range()/static_cast<double>(steps);
    vector<vector<double>> F;
    vector<double> F_start = {f.limits.lower, 0.0};
    F.push_back(F_start);
    for (uint64_t i = 1; i < steps; i++) {
        double x = f.limits.lower + i*step;
        double y = f.evaluate(x)*step + F.back()[1];

        vector<double> F_at_x = {x, y};
        F.push_back(F_at_x);
    }
    double x_end = f.limits.upper;
    double x_end_in_f = nextafter(x_end, f.limits.lower);
    vector<double> F_end = {
        x_end,
        f.evaluate(x_end_in_f)*step + F.back()[1]
    };
    F.push_back(F_end);
    return Func1(F);
}

Func1 inverse(const Func1 &f, uint64_t steps) {
    double step = f.limits.range()/static_cast<double>(steps);
    vector<vector<double>> fvec_inv;
    vector<double> f_inv_start = {
        f.evaluate(f.limits.lower),
        f.limits.lower
    };
    fvec_inv.push_back(f_inv_start);
    for (uint64_t i = 1; i < steps; i++) {
        double x = f.limits.lower + i*step;
        double y = f.evaluate(x);
        vector<double> f_inv_at_x = {y, x};
        fvec_inv.push_back(f_inv_at_x);
    }
    double x_end = f.limits.upper;
    double x_end_in_f = nextafter(x_end, f.limits.lower);
    vector<double> f_inv_end = {f.evaluate(x_end_in_f), x_end};
    fvec_inv.push_back(f_inv_end);
    return Func1(fvec_inv);
}

Func1 derivative(const Func1 &f, uint64_t steps) {
    double step = f.limits.range()/static_cast<double>(steps);
    vector<vector<double>> fvec_deriv;
    for (uint64_t i = 0; i < (steps-1); i++) {
        double x = f.limits.lower + i*step;
        double y = (f.evaluate(x+step) - f.evaluate(x))/step;
        vector<double> f_inv_at_x = {x, y};
        fvec_deriv.push_back(f_inv_at_x);
    }
    double x_end = f.limits.upper;
    double x_end_margin = step*1e-9;
    double y_end = (
        f.evaluate(x_end-x_end_margin) -
        f.evaluate(x_end-x_end_margin-step))/step;
    vector<double> f_inv_end = {x_end, y_end};
    fvec_deriv.push_back(f_inv_end);
    return Func1(fvec_deriv);
}

bool value_flips_sign(const Func1 &f, uint64_t steps) {
    double step = f.limits.range()/static_cast<double>(steps);
    bool flip = false;
    for (uint64_t i = 0; i < (steps-1); i++) {
        double x = f.limits.lower + i*step;
        if ((f.evaluate(x) >= 0.0) != (f.evaluate(x+step) >= 0.0))
            flip = true;
    }
    return flip;
}

std::vector<std::vector<double>> polynom3(
    double c3,
    double c2,
    double c1,
    double c0,
    double start,
    double end,
    uint64_t steps
) {
    const double increment = (end - start)/static_cast<double>(steps - 1);
    std::vector<std::vector<double>> out;
    for (uint64_t i = 0; i < (steps - 1); i++) {
        const double x = start + increment*static_cast<double>(i);
        const double y = x*x*x*c3 + x*x*c2 + x*c1 + c0;
        out.push_back({x, y});
    }
    const double x = end;
    const double y = x*x*x*c3 + x*x*c2 + x*c1 + c0;
    out.push_back({x, y});
    return out;
}

}  // namespace function
}  // namespace relleums
