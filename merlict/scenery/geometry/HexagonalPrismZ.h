// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_HEXAGONALPRISMZ_H_
#define SCENERY_GEOMETRY_HEXAGONALPRISMZ_H_

#include <math.h>
#include "merlict/scenery/geometry/PrismZ.h"

namespace merlict {

const Vec3 VEC3_UNIT_U = VEC3_UNIT_Y;
const Vec3 VEC3_UNIT_V =
    Vec3(1., 0., 0.) * +sin(2./3.*M_PI) +
    Vec3(0., 1., 0.) * cos(2./3.*M_PI);
const Vec3 VEC3_UNIT_W =
    Vec3(1., 0., 0.) * -sin(2./3.*M_PI) +
    Vec3(0., 1., 0.) * cos(2./3.*M_PI);

//            /|\ Y-axis                                                  //
//             |                                                          //
//          ___|___                                                       //
//  X-axis /   |   \                                                      //
//  /_____/____|    \                                                     //
//  \     \         /                                                     //
//         \_______/                                                      //


class HexagonalPrismZ :public PrismZ {
 private:
    double outer_radius;
    double inner_radius;
 public:
    void set_outer_radius(const double outer_radius);
    bool is_inside(const Vec3* vec)const;
    double get_area()const;
    double get_outer_radius()const;
 private:
    void assert_outer_radius_positive(const double outer_radius)const;
};

}  // namespace merlict

#endif  // SCENERY_GEOMETRY_HEXAGONALPRISMZ_H_
