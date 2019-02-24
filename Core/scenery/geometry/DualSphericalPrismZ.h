// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_DUALSPHERICALPRISMZ_H_
#define SCENERY_GEOMETRY_DUALSPHERICALPRISMZ_H_

#include "PrismZ.h"
#include "Core/Vec2.h"

namespace relleums {

class DualSphericalPrismZ :public PrismZ {
 private:
    double circle_distance;
    double circle_radius;
    double hight;
    double width;
    Vec2 circ1;
    Vec2 circ2;

 public:
    void set_x_hight_and_y_width(
        const double hight,
        const double width);
    double get_hight()const;
    double get_width()const;
    double get_enclosing_radius()const;
    double get_circle_radius()const;
    double get_area()const;
    bool is_inside(const Vec3* vec)const;
};

}  // namespace relleums

#endif  // SCENERY_GEOMETRY_DUALSPHERICALPRISMZ_H_
