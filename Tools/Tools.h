// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_TOOLS_H_
#define TOOLS_TOOLS_H_

#include <string>
#include <vector>

namespace relleums {

double deg2rad(double angle_in_Deg);
double rad2deg(double angle_in_Rad);
bool is_odd(const int n);
bool is_even(const int n);
double get_solid_angle_for_opening_angle(const double theta);
double get_mean_along_column(
    const std::vector<std::vector<double>> &table,
    const unsigned int column);
std::string get_print(const std::vector<std::vector<double>> &table);

}  // namespace relleums

#endif  // TOOLS_TOOLS_H_
