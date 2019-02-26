// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/geometry/DualSphericalPrismZ.h"
#include <math.h>

namespace relleums {

void DualSphericalPrismZ::set_x_hight_and_y_width(
    const double hight,
    const double width
) {
    this->hight = hight;
    this->width = width;
    circle_distance = (width*width - hight*hight)/(2.0*hight);
    circle_radius = circle_distance/2.0 + hight/2.0;

    circ1 = Vec2(+circle_distance/2.0, 0.0);
    circ2 = Vec2(-circle_distance/2.0, 0.0);
}

bool DualSphericalPrismZ::is_inside(const Vec3* vec)const {
    Vec2 v2c1 = circ1 - Vec2(vec->x, vec->y);
    Vec2 v2c2 = circ2 - Vec2(vec->x, vec->y);

    return  v2c1.norm_is_less_equal_than(circle_radius) &&
            v2c2.norm_is_less_equal_than(circle_radius);
}

double DualSphericalPrismZ::get_hight()const {
    return hight;
}

double DualSphericalPrismZ::get_width()const {
    return width;
}

double DualSphericalPrismZ::get_enclosing_radius()const {
    return width*0.5;
}

double DualSphericalPrismZ::get_circle_radius()const {
    return circle_radius;
}

double DualSphericalPrismZ::get_area()const {
    double opening_angle_of_circle = 2.0*asin(width/(2.0*circle_radius));
    return 2.0*(circle_radius*circle_radius/2.0)*
        (opening_angle_of_circle - sin(opening_angle_of_circle));
}

}  // namespace relleums