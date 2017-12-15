// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/LinInterpol.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include "Tools/StringTools.h"
using std::vector;
using std::string;

namespace Function {

LinInterpol::LinInterpol() {}

LinInterpol::LinInterpol(const vector<vector<double>>& xy) {
    init(xy);
}

void LinInterpol::init(const vector<vector<double>>& xy) {
    func.reserve(xy.size());

    for (unsigned int i = 0; i < xy.size(); i++) {
        assert_table_two_columns(xy, i);
        LinInterpol::Point p = {
            xy[i][0],
            xy[i][1],
            slope_in_table_in_row(xy, i)
        };
        func.push_back(p);
    }

    assert_func_is_not_empty();
    sort_function_arguments();
    assert_no_duplicate_argument_on_sorted_arguments();

    limits_ = Limits(func.front().x, func.back().x);
}

double LinInterpol::slope_in_table_in_row(
    const vector<vector<double>> &xy,
    const unsigned int row
)const {
        if (row != xy.size()-1)
            return (xy[row][1] -xy[row+1][1]) / (xy[row][0] - xy[row+1][0]);
        else
            return 0.0;
}

void LinInterpol::assert_table_two_columns(
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

double LinInterpol::evaluate(const double x)const {
    limits_.assert_contains(x);
    vector<LinInterpol::Point>::const_iterator upper = get_upper_bound(x);
    assert_upper_bound_and_argument_in_range(upper, x);
    return interpolate_linear(*(upper-1), x);
}

vector<LinInterpol::Point>::const_iterator LinInterpol::get_upper_bound(
    double arg
)const {
    return std::upper_bound(func.begin(), func.end(), arg, comp_upp);
}

void LinInterpol::assert_upper_bound_and_argument_in_range(
    const vector<LinInterpol::Point>::const_iterator it,
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

void LinInterpol::sort_function_arguments() {
    std::sort(func.begin(), func.end(), LinInterpol::Point());
}

void LinInterpol::assert_no_duplicate_argument_on_sorted_arguments()const {
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

double LinInterpol::interpolate_linear(
    const LinInterpol::Point p0,
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

bool LinInterpol::comp_upp(const double x, const LinInterpol::Point P) {
    return P.x > x;
}

string LinInterpol::exception_header()const {
    return "LinInterpol:\n";
}

void LinInterpol::assert_func_is_not_empty()const {
    if (func.size() == 0) {
        std::stringstream info;
        info << exception_header();
        info << "Function is empty.\n";
        throw std::invalid_argument(info.str());
    }
}

double LinInterpol::max()const {
    double max_y = func.front().y;
    for (Point p : func) if (p.y > max_y) max_y = p.y;
    return max_y;
}

double LinInterpol::min()const {
    double min_y = func.front().y;
    for (Point p : func) if (p.y < min_y) min_y = p.y;
    return min_y;
}

}  // namespace Function
