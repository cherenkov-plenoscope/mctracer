// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_TOOLS_H_
#define MERLICT_TOOLS_H_

#include <string>

namespace merlict {

double deg2rad(double angle_in_Deg);

double rad2deg(double angle_in_Rad);

bool is_odd(const int n);

bool is_even(const int n);

double get_solid_angle_for_opening_angle(const double theta);

void write_text_to_file(
    const std::string &text,
    const std::string &path);

}  // namespace merlict

#endif  // MERLICT_TOOLS_H_
