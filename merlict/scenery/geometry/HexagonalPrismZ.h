// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_HEXAGONALPRISMZ_H_
#define SCENERY_GEOMETRY_HEXAGONALPRISMZ_H_

#include "merlict/scenery/geometry/PrismZ.h"

namespace merlict {

class HexagonalPrismZ :public PrismZ {
 private:
    static const Vec3 UNIT_U;
    static const Vec3 UNIT_V;
    static const Vec3 UNIT_W;

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
