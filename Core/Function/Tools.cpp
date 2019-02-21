// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/Tools.h"
#include <math.h>
#include <vector>
using std::vector;

namespace relleums {
namespace Function {

LinInterpol get_integral(const Func1D &f, unsigned int steps) {
    double step = f.limits().range()/static_cast<double>(steps);
    vector<vector<double>> F;
    vector<double> F_start = {f.limits().lower(), 0.0};
    F.push_back(F_start);
    for (unsigned int i = 1; i < steps; i++) {
        double x = f.limits().lower() + i*step;
        double y = f.evaluate(x)*step + F.back()[1];

        vector<double> F_at_x = {x, y};
        F.push_back(F_at_x);
    }
    double x_end = f.limits().upper();
    double x_end_in_f = nextafter(x_end, f.limits().lower());
    vector<double> F_end = {
        x_end,
        f.evaluate(x_end_in_f)*step + F.back()[1]
    };
    F.push_back(F_end);
    return LinInterpol(F);
}

LinInterpol get_inverse(const Func1D &f, unsigned int steps) {
    double step = f.limits().range()/static_cast<double>(steps);
    vector<vector<double>> fvec_inv;
    vector<double> f_inv_start = {
        f.evaluate(f.limits().lower()),
        f.limits().lower()
    };
    fvec_inv.push_back(f_inv_start);
    for (unsigned int i = 1; i < steps; i++) {
        double x = f.limits().lower() + i*step;
        double y = f.evaluate(x);
        vector<double> f_inv_at_x = {y, x};
        fvec_inv.push_back(f_inv_at_x);
    }
    double x_end = f.limits().upper();
    double x_end_in_f = nextafter(x_end, f.limits().lower());
    vector<double> f_inv_end = {f.evaluate(x_end_in_f), x_end};
    fvec_inv.push_back(f_inv_end);
    return LinInterpol(fvec_inv);
}

LinInterpol get_derivative(const Func1D &f, unsigned int steps) {
    double step = f.limits().range()/static_cast<double>(steps);
    vector<vector<double>> fvec_deriv;
    for (unsigned int i = 0; i < (steps-1); i++) {
        double x = f.limits().lower() + i*step;
        double y = (f.evaluate(x+step) - f.evaluate(x))/step;
        vector<double> f_inv_at_x = {x, y};
        fvec_deriv.push_back(f_inv_at_x);
    }
    double x_end = f.limits().upper();
    double x_end_margin = step*1e-9;
    double y_end = (
        f.evaluate(x_end-x_end_margin) -
        f.evaluate(x_end-x_end_margin-step))/step;
    vector<double> f_inv_end = {x_end, y_end};
    fvec_deriv.push_back(f_inv_end);
    return LinInterpol(fvec_deriv);
}

bool value_flips_sign(const Func1D &f, unsigned int steps) {
    double step = f.limits().range()/static_cast<double>(steps);
    bool flip = false;
    for (unsigned int i = 0; i < (steps-1); i++) {
        double x = f.limits().lower() + i*step;
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

}  // namespace Function
}  // namespace relleums
