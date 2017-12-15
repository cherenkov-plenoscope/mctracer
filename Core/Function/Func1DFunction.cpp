// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/Func1DFunction.h"
#include <sstream>
using std::string;
using std::vector;


namespace Function {

Func1D::Func1D() {}

Func1D::Func1D(const Limits &_limits): limits_(_limits) {}

Func1D::~Func1D() {}

vector<vector<double>> Func1D::sample(const unsigned int N)const {
    double arg = limits_.lower();
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

double Func1D::mean(const unsigned int N)const {
    vector<vector<double>> x_vs_y   = sample(N);
    double y_mean = 0.0;
    for (vector<double> point : x_vs_y)
        y_mean = y_mean + point.at(1);
    return y_mean/x_vs_y.size();
}

double Func1D::increment_for_steps(const unsigned int N)const {
    return limits_.range()/static_cast<double>(N);
}

Limits Func1D::limits()const {
    return limits_;
}

string Func1D::str()const {
    std::stringstream out;
    out.precision(2);
    out << limits_.str() << " ";
    vector<vector<double>> table = sample(3);
    for (vector<double> xy : table) {
        out << "f(" << xy.at(0) << ")=" << xy.at(1) <<", ";
    }
    return out.str();
}

}  // namespace Function
