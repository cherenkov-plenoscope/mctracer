// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/Func1DFunction.h"
#include <sstream>
using std::string;
using std::vector;


namespace Function {

Func1D::Func1D() {}

Func1D::Func1D(const Limits &_limits): limits(_limits) {}

Func1D::~Func1D() {}

vector<vector<double>> Func1D::get_samples(const unsigned int N)const {
    double arg = limits.get_lower();
    const double increment = increment_for_steps(N);
    vector<vector<double>> table;
    table.reserve(N);
    for (unsigned int i = 0; i < N; i++) {
        const double value = evaluate(arg);
        vector<double> row = {arg, value};

        table.push_back(row);
        arg = arg + increment;
    }
    return table;
}

double Func1D::get_mean(const unsigned int N)const {
    vector<vector<double>> x_vs_y   = get_samples(N);
    double y_mean = 0.0;
    for (vector<double> point : x_vs_y)
        y_mean = y_mean + point.at(1);
    return y_mean/x_vs_y.size();
}

double Func1D::increment_for_steps(const unsigned int N)const {
    return limits.get_range()/static_cast<double>(N);
}

Limits Func1D::get_limits()const {
    return limits;
}

string Func1D::str()const {
    std::stringstream out;
    out.precision(2);
    out << limits.str() << " ";
    vector<vector<double>> table = get_samples(3);
    for (vector<double> xy : table) {
        out << "f(" << xy.at(0) << ")=" << xy.at(1) <<", ";
    }
    return out.str();
}

}  // namespace Function
