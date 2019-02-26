// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_TOOLS_H_
#define CORE_TOOLS_H_

namespace merlict {

double deg2rad(double angle_in_Deg);
double rad2deg(double angle_in_Rad);
bool is_odd(const int n);
bool is_even(const int n);
double get_solid_angle_for_opening_angle(const double theta);

}  // namespace relleums

#endif  // CORE_TOOLS_H_
