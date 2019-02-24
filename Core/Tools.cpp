// Copyright 2014 Sebastian A. Mueller
#include "Core/Tools.h"
#include <cmath>

namespace relleums {

double deg2rad(double angle_in_Deg) {
    return (M_PI*angle_in_Deg)/180.0;
}

double rad2deg(double angle_in_Rad) {
    return (angle_in_Rad/M_PI)*180.0;
}

bool is_odd(const int n) {
    return n%2 != 0;
}

bool is_even(const int n) {
    return !is_odd(n);
}

double get_solid_angle_for_opening_angle(const double theta) {
    //-------
    //         _ _        _______________
    //       h _|_   ____/__/__/_|/__/__/\____ <---cap area we are looking for//
    //            __/     \ theta|            \__                             //
    //           /         \     |               \                            //
    //          |           \    |                |                           //
    //         |             \   |                |
    //         |        r=1.0 \  |                 |
    //        |                \ |                  |
    //        |                 \|                  |
    //        |                  x                  |
    //        |             unit sphere             |
    //        |                                    |
    //         |                                   |
    //         |                                   |
    //          |                                 |
    //           \__                           __/
    //              \___                  ____/
    //                  \________________/
    //
    //
    double h = (1.0 - cos(theta));
    return 2.0*M_PI*h;
}

}  // namespace relleums
