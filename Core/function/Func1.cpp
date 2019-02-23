// Copyright 2014 Sebastian A. Mueller
#include "Core/function/Func1.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include "Tools/StringTools.h"
#include "Core/function/function.h"
using std::vector;
using std::string;

namespace relleums {
namespace function {

bool Point::operator()(const Point &l, const Point &r) {
    return l.x < r.x;
}

Func1::Func1() {}

Func1::Func1(const vector<vector<double>>& xy) {
    init(xy);
}

void Func1::init(const vector<vector<double>>& xy) {
    func.reserve(xy.size());

    for (unsigned int i = 0; i < xy.size(); i++) {
        assert_table_two_columns(xy, i);
        Point p = {
            xy[i][0],
            xy[i][1],
            slope_in_table_in_row(xy, i)
        };
        func.push_back(p);
    }

    assert_func_is_not_empty();
    sort_function_arguments();
    assert_no_duplicate_argument_on_sorted_arguments();

    limits = Limits(func.front().x, func.back().x);
}

double Func1::slope_in_table_in_row(
    const vector<vector<double>> &xy,
    const unsigned int row
)const {
        if (row != xy.size()-1)
            return (xy[row][1] -xy[row+1][1]) / (xy[row][0] - xy[row+1][0]);
        else
            return 0.0;
}

void Func1::assert_table_two_columns(
    const vector<vector<double>> &xy_table,
    const unsigned int row
)const {
    if (xy_table.at(row).size() != 2) {
        std::stringstream info;
        info << exception_header();
        info << "Expected vec of vec matrix of shape N x 2, but actual:\n";
        info << "Row " << row << " has " << xy_table.at(row).size();
        info << " cols.\n";
        throw std::invalid_argument(info.str());
    }
}

double Func1::evaluate(const double x)const {
    limits.assert_contains(x);
    vector<Point>::const_iterator upper = get_upper_bound(x);
    assert_upper_bound_and_argument_in_range(upper, x);
    return interpolate_linear(*(upper-1), x);
}

vector<Point>::const_iterator Func1::get_upper_bound(
    double arg
)const {
    return std::upper_bound(func.begin(), func.end(), arg, comp_upp);
}

void Func1::assert_upper_bound_and_argument_in_range(
    const vector<Point>::const_iterator it,
    const double arg
)const {
    if (
        it == func.begin() ||
        ((it == func.end()) &&
        arg > (func.end() - 1)->x)
    ) {
        std::stringstream info;
        info << exception_header();
        info << "Expected argument to be in range of arguments ";
        info << "[ " << func.begin()->x << ", " << (func.end()-1)->x << " ] ";
        info << ", but actual argument = " << arg << "\n";
        throw std::out_of_range(info.str());
    }
}

void Func1::sort_function_arguments() {
    std::sort(func.begin(), func.end(), Point());
}

void Func1::assert_no_duplicate_argument_on_sorted_arguments()const {
    // there must not be an argument twice! This is enforced here by throwing an
    // exception in case it is. Since the list is now sorted, same arguments
    // will be next to each other. We go through the vector and compare each
    // element with its upper neighbor.
    for (unsigned int i = 0; i < func.size()-1; i++) {
        if (func.at(i).x == func.at(i+1).x) {
            std::stringstream info;
            info << exception_header();
            info << "Expected argument " << func.at(i).x;
            info << " to be unique, but actual its not, see position: " << i;
            info << "\n";
            throw std::logic_error(info.str());
        }
    }
}

double Func1::interpolate_linear(
    const Point p0,
    const double x
)const {
    //  Point p0: (x0, y0)
    //  Point p1: (x1, y1)
    //  the line in between Point p0 and p1 is
    //  f(x) = slope*x + y_intercept
    //
    //  slope = (y0 - y1) / (x0 - x1)
    //  y_intercept = y0 - slope*x0
    return p0.slope*x + p0.y - p0.slope*p0.x;
}

bool Func1::comp_upp(const double x, const Point P) {
    return P.x > x;
}

string Func1::exception_header()const {
    return "LinInterpol:\n";
}

void Func1::assert_func_is_not_empty()const {
    if (func.size() == 0) {
        std::stringstream info;
        info << exception_header();
        info << "Function is empty.\n";
        throw std::invalid_argument(info.str());
    }
}

double Func1::max()const {
    double max_y = func.front().y;
    for (Point p : func) if (p.y > max_y) max_y = p.y;
    return max_y;
}

double Func1::min()const {
    double min_y = func.front().y;
    for (Point p : func) if (p.y < min_y) min_y = p.y;
    return min_y;
}

string Func1::str()const {
    std::stringstream out;
    out.precision(2);
    out << limits.str() << " ";
    vector<vector<double>> table = sample(*this, 3);
    for (vector<double> xy : table) {
        out << "f(" << xy.at(0) << ")=" << xy.at(1) <<", ";
    }
    return out.str();
}

}  // namespace function
}  // namespace relleums
